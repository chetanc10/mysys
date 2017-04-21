#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "xml.h"

#if ENABLE_DEC_DBG
#define DEBUG_DEC PRINTF
#else
#define DEBUG_DEC(...)
#endif

#define SKIP_SPACES(ptr) while(ptr && isspace((int)*ptr)) { ptr++; }
#define SKIP_LETTERS(ptr) \
	while(ptr && *ptr && !isspace((int)*ptr) && (*ptr !='/') && (*ptr !='>')) { ptr++; }

extern XML_NODE_T *rootNode;

char *xstrndup(char *src, size_t len)
{
	char *dest = NULL;
	size_t dLen = len;

	if(!src) {
		return NULL;
	}

	dLen = ((strlen(src) < dLen) ? strlen(src) : dLen);

	dLen = (!dLen)?strlen(src):dLen;

	dest = (char *)calloc(1, dLen + 1); /*+1 for NULL char to end-string*/

	return strncpy(dest, src, dLen);
}

static char *decBasePtr;
static char *decPtr;

#define BET_TOKEN 0x01
#define GET_TOKEN 0x02
static inline char * findStrToken(char *str, const char *token, uint8_t checkType)
{
	char *ptr = strstr(str, token);

	if((!ptr) || (ptr == str)) {
		return NULL;
	}

	if(checkType & BET_TOKEN) {
		return (char *)!NULL;
	}

	return xstrndup(str, (size_t)(ptr - str));
}

#define checkStrToken(str, token) findStrToken(str, token, BET_TOKEN)
#define getStrToken(str, token) findStrToken(str, token, GET_TOKEN)

#if USING_TAG_ATTR
static xml_status_t getNodeAttr(XML_NODE_T **xmlNode, char **tag)
{
	NODE_ATTR_T *curAttr = NULL, *lastAttr = NULL;
	char *ptr, *tPtr, *tEnd = strchr(*tag, '>');

	if(!(*tag)) {
		printf("What String is this?\n");
		return XML_ERR_BAD_TAG;
	}

   	tEnd -= (*(tEnd-1) == '/');
	
	tPtr = *tag;
	ptr = getStrToken(tPtr, "=");
	if(!ptr) {
		//printf("No attributes for %s\n", (*xmlNode)->tagField);
		return XML_SUCCESS;
	}

	DEBUG_DEC ("finding attrs for %s\n", (*xmlNode)->tagField);
	do {
		curAttr = calloc(1, sizeof(NODE_ATTR_T));
		if(!curAttr) {
			printf("doomed memory\n");
			return XML_ERR_ALLOC;
		}

		curAttr->attrName = ptr;
		DEBUG_DEC ("%s=", curAttr->attrName);

		ptr = strchr(tPtr, '=');
		if(!ptr) {
			DEBUG_DEC ("No more attributes\n");
			break;
		}
		ptr = strchr(ptr, '"');
		if((!ptr) || (!strchr(ptr+1, '"'))) {
			printf("Give something proper to chew!!\n");
			return XML_ERR_BAD_ATTR;
		}

		curAttr->attrValue = getStrToken(++ptr, "\"");
		if(!curAttr->attrValue) {
			printf("Error getting attrValue\n");
			return XML_ERR_BAD_ATTR;
		}
		DEBUG_DEC ("%s ", curAttr->attrValue);

		if(!(*xmlNode)->attrList) {
			(*xmlNode)->attrList = curAttr;
		} else {
			lastAttr->next = curAttr;
		}
		lastAttr = curAttr;

		(*xmlNode)->numOfAttr++;
		SKIP_LETTERS(ptr);
		SKIP_SPACES(ptr);
		tPtr = ptr;
	} while((tPtr < tEnd) && (NULL != (ptr = getStrToken(tPtr, "="))));

	DEBUG_DEC (" | attr listed for: %s\n", (*xmlNode)->tagField);
	*tag = tEnd;

	return XML_SUCCESS;
}
#endif

static void populateNodeInTree(XML_NODE_T **prevNode, XML_NODE_T **curNode)
{
	if(*prevNode) {
		if((*prevNode)->nodeType == NODE_PARENT) {
			DEBUG_DEC ("add as child\n");
			if(!(*prevNode)->children) {
				DEBUG_DEC ("1st child\n");
				(*prevNode)->numOfChildren = 0;
				(*prevNode)->children = (*curNode);
			} else {
				XML_NODE_T *link = (*prevNode)->children;
				for(; link->next; link = link->next);
				link->next = (*curNode);
			}
			(*curNode)->parent = (*prevNode);
			((*prevNode)->numOfChildren)++;
			DEBUG_DEC ("%s child to %s\n", (*curNode)->tagField, (*curNode)->parent->tagField);
		} else {
		    if((*prevNode)->parent) {
		        ((*prevNode)->parent->numOfChildren)++;
		    }
			(*prevNode)->next = (*curNode);
			(*curNode)->parent = (*prevNode)->parent;
			DEBUG_DEC ("%s next to %s\n", (*prevNode)->next->tagField, (*prevNode)->tagField);
		}
	}
}

static xml_status_t 
getNodeFromNewTag(XML_NODE_T **prevNode, XML_NODE_T **curNode, node_type_t nodeType)
{
	char *tPtr = decPtr;

	(*curNode) = calloc(1, sizeof(XML_NODE_T));
	if(!(*curNode)) {
		printf("memory alloc err for node\n");
		return XML_ERR_ALLOC;
	}

	/*we dont need check NODE_SINGLE, since it gets updated when '/>' comes*/
	/*after useset decPtr to '>' for other than PARENT;
	 *in PARENT point next to tagName
	 *for content, point to next tag opening*/
	switch(nodeType) {
		case NODE_PARENT:
			DEBUG_DEC ("PARENT: ");
			DEBUG_DEC ("%s", decPtr);
			SKIP_LETTERS(decPtr); /*skip upto end of tag name*/
			if (!*decPtr) {
				printf ("Bad Tag.. not XML format!\r\n");
				goto OFF_TAG;
			}
			/*copy name to tagField*/
			(*curNode)->tagField = xstrndup(tPtr, (size_t)(decPtr - tPtr));
			break;
#if USING_COMMENTS
		case NODE_COMMENT:
			DEBUG_DEC ("COMMENT\n");
			(*curNode)->tagField = getStrToken(decPtr, "-->");
			decPtr = strstr(decPtr, "-->") + 2;
			break;
#endif
#if USING_CDATA
		case NODE_CDATA:
			DEBUG_DEC ("CDATA\n");
			(*curNode)->tagField = getStrToken(decPtr, "]]>");
			decPtr = strstr(decPtr, "]]>") + 2;
			break;
#endif
#if USING_INSTR
		case NODE_INSTR:
			DEBUG_DEC ("INSTR\n");
			(*curNode)->tagField = getStrToken(decPtr, "?>");
			decPtr = strstr(decPtr, "?>") + 2;
			break;
#endif
		case NODE_CONTENT:
			DEBUG_DEC ("CONTENT\n");
			(*curNode)->tagField = getStrToken(decPtr, "<");
			decPtr = strchr(decPtr, '<');
			break;
		case NODE_SINGLE: /*we won't have this ever, but have fail-safe*/
			break;
		case NODE_MIN:
		case NODE_MAX:
		default:
			free((*curNode));
			return XML_ERR_BAD_NODETYPE;
	}

	if(!(*curNode)->tagField) {
		printf("tagField memory error\n");
OFF_TAG:
		free((*curNode));
		return XML_ERR_ALLOC;
	}

	(*curNode)->nodeType = nodeType;

	populateNodeInTree(prevNode, curNode);

	return XML_SUCCESS;
}

static xml_status_t dec_buildXmlTree(XML_NODE_T **xmlNode)
{
	XML_NODE_T *curNode = NULL, *prevNode = NULL;
	xml_status_t status;

	(*xmlNode) = NULL;

NEW_TAG:
	SKIP_SPACES(decPtr);
	if(*decPtr == '<') {
		node_type_t type;
		decPtr++;
		if(*decPtr == '/') {
			char *ptr, *tName;
			/*should be a tag-closing: </tagName>*/
			decPtr++;
			ptr = strchr(decPtr, '>');
			if(!ptr) {
				printf("Syntax error: bad tag-closing\n");
				return XML_ERR_BAD_TAG;
			} else if(!prevNode) {
				printf("Syntax error: bad tag-closing.. no prev\n");
				return XML_ERR_BAD_TAG;
			}

			if(curNode->nodeType == NODE_PARENT) {
				tName = curNode->tagField;
			} else if(curNode->parent) {
				tName = curNode->parent->tagField;
				curNode = curNode->parent;
			} else {
				printf("Unknown error\n");
				return XML_FAILURE;
			}

			if((strlen(tName) != (size_t)(ptr - decPtr)) || \
					(XML_SUCCESS != strncmp(tName, decPtr, (size_t)(ptr-decPtr)))) {
				printf("NODE Mismatch at closing-tag: %s\n", tName);
				return XML_ERR_BAD_TAG;
			}

			decPtr = ptr+1;
			DEBUG_DEC ("NODE FULL: %s\n", tName);
			if(curNode->parent) {
				prevNode = curNode = curNode->parent;
			} else {
				prevNode = curNode;
			}
			goto NEW_TAG;
		}
#if USING_INSTR
		else if(*decPtr == '?') {
			/*Instruction tag*/
			decPtr++;
			type = NODE_INSTR;
		}
#endif
#if USING_COMMENTS
		else if(XML_SUCCESS == strncmp(decPtr, "!--", 3)) {
			/*Comment tag*/
			decPtr += 3;
			SKIP_SPACES(decPtr);
			type = NODE_COMMENT;
		}
#endif
#if USING_CDATA
		else if(XML_SUCCESS == strncmp(decPtr, "![CDATA[", 8)) {
			/*Cdata tag*/
			decPtr += 8;
			type = NODE_CDATA;
		}
#endif
		else {
			/*Parent tag.. default*/
			type = NODE_PARENT;
		}

		/*a new tag begins*/
		status = getNodeFromNewTag(&prevNode, &curNode, type);
		if(XML_SUCCESS != status) {
			printf("node addition failed\n");
			return status;
		}

		(*xmlNode) = !(*xmlNode)?curNode:(*xmlNode);
		prevNode = curNode;

		/*Now decPtr points to <space> or '/' or '>'*/
		DEBUG_DEC ("node: %s\n", curNode->tagField);
	} else {
		if((size_t)(decPtr - decBasePtr) == strlen(decBasePtr)) {
			DEBUG_DEC ("DONE!!\n");
			return XML_SUCCESS;
		}
		/*we have a content of parent now.. not a child*/
		status = getNodeFromNewTag(&prevNode, &curNode, NODE_CONTENT);
		if(XML_SUCCESS != status) {
				printf("content node addition failed\n");
				return status;
		}
		DEBUG_DEC ("content: %s\n", curNode->tagField);
		/*let prevNode be as it is, this node is just content..
		 *prevNode reference shall be the parent of this content*/
		goto NEW_TAG;
	}

	SKIP_SPACES(decPtr);
	if(*decPtr == '>') { /*Seems tag name ended*/
		/*this parent might have children/content/both/end-of-itself*/
		decPtr++;
		goto NEW_TAG;
	} else if(*decPtr == '/') { /*tag ended is self-tag?*/
		DEBUG_DEC ("self tag: %s\n", curNode->tagField);
		goto SELF_TAG;
	} else if((curNode->nodeType == NODE_PARENT)) {
#if USING_TAG_ATTR
		/*if we have attributes, get them*/
		status = getNodeAttr(&curNode, &decPtr);
		if(XML_SUCCESS != status) {
			printf("Erroneous attribute attempted!\n");
			return status;
		}
#endif
	}

	if(*decPtr == '/') {
SELF_TAG:
		/*might be a self-ending-tag*/
		if(*(decPtr+1) !='>') {
			printf("Invalid Tag End for %s\n", curNode->tagField);
			return XML_ERR_BAD_TAG;
		}
		decPtr++;
		curNode->nodeType = NODE_SINGLE;
		DEBUG_DEC ("single node: %s\n", curNode->tagField);
	}
	
	if(*decPtr == '>') {
		decPtr++;
	}

	goto NEW_TAG;
}

xml_status_t decode_xml_pkt(char *str, XML_NODE_T **xmlNode)
{
	char *xStr = str;

	if(!xStr || (strlen(xStr) >= MAX_RX_XML_PKT_SIZE)) {
		printf("Invalid packet\n");
		return XML_FAILURE;
	}

	if((*xStr != '<') && !(xStr = strchr(xStr, '<'))) {
		printf("No XML start-tag\n");
		return XML_FAILURE;
	}

	decPtr = decBasePtr = xStr;

	if(rootNode) {
	    destroyXMLtree();
	}

	if(XML_SUCCESS != dec_buildXmlTree(xmlNode)) {
	    destroyXMLtree();
		return XML_FAILURE;
	}

	rootNode = *xmlNode;
#if 0
	{
	/* To confirm decode operation is successful, 
	 * compare the following encoded buffer with the 'str' passed to decoder*/
		char *ptr;
		if(!encode_xml_pkt(&ptr)) {
			PRINTF ("xmlBuf: %s\r\n", ptr);
		}
	}
#endif

	return XML_SUCCESS;
}
