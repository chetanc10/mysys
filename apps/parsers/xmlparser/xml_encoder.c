#include <stdio.h>
#include <stdlib.h>

#include "xml.h"

#if ENABLE_ENC_DBG
#define DEBUG_ENC PRINTF
#else
#define DEBUG_ENC(...)
#endif

#define IS_NODE_TYPE_VALID(nodeType) \
	((nodeType > NODE_MIN) && (nodeType < NODE_MAX))

#define IS_INVALID_NODE_TYPE(type) (!IS_NODE_TYPE_VALID(type))

#define IS_NODE_SINGLE_VALID(node) \
	((node->nodeType != NODE_SINGLE) || !(node->children))

#if USING_TAG_ATTR
#define IS_HAVING_ATTR(node) (node->numOfAttr && node->attrList)
#endif

XML_NODE_T *rootNode = NULL;
static char *encodedPktStr;

#if USING_TAG_ATTR
static xml_status_t putNodeAttr(XML_NODE_T *xmlNode, char **encPtr)
{
	uint8_t n = xmlNode->numOfAttr;
	NODE_ATTR_T *attrListPtr = xmlNode->attrList;

	for(; n && attrListPtr; n--, attrListPtr = attrListPtr->next) {
		*encPtr += sprintf(*encPtr, " %s=\"%s\"", \
				attrListPtr->attrName, attrListPtr->attrValue);
		DEBUG_ENC ("attr::\n\r%s\n\r", encodedPktStr);
	}

	return (n || attrListPtr) ? XML_ERR_BAD_ATTR:XML_SUCCESS;
}
#endif

static xml_status_t enc_buildXmlBuf(char **encPtr)
{
	XML_NODE_T *curNode = rootNode;

NEW_NODE:
	if(curNode->nodeType != NODE_CONTENT) {
		*encPtr += sprintf(*encPtr, "<");
	}

	switch(curNode->nodeType) {
#if USING_COMMENTS
		case NODE_COMMENT: *encPtr += sprintf(*encPtr, "!--"); break;
#endif
#if USING_CDATA
		case NODE_CDATA: *encPtr += sprintf(*encPtr, "![CDATA["); break;
#endif
#if USING_INSTR
		case NODE_INSTR: *encPtr += sprintf(*encPtr, "?"); break;
#endif
		default: break;
	}

	*encPtr += sprintf(*encPtr, "%s", curNode->tagField);
	DEBUG_ENC ("1:\n\r%s\n\r", encodedPktStr);

	switch(curNode->nodeType) {
		case NODE_SINGLE:
		case NODE_PARENT:
#if USING_TAG_ATTR
			if(IS_HAVING_ATTR(curNode) && \
					(XML_SUCCESS != putNodeAttr(curNode, encPtr))) {
				return XML_FAILURE;
			}
#endif
			*encPtr += sprintf(*encPtr, \
					(curNode->nodeType == NODE_SINGLE)?"/>":">");
			break;
#if USING_COMMENTS
		case NODE_COMMENT:
			*encPtr += sprintf(*encPtr, "-->");
			break;
#endif
#if USING_CDATA
		case NODE_CDATA:
			*encPtr += sprintf(*encPtr, "]]>");
			break;
#endif
#if USING_INSTR
		case NODE_INSTR:
			*encPtr += sprintf(*encPtr, "?>");
			break;
#endif
		default:
			break;
	}

	DEBUG_ENC ("2:\n\r%s\n\r", encodedPktStr);

	if(((*encPtr - encodedPktStr) > MAX_TX_XML_PKT_SIZE)) {
		printf("OverFlown xml encoded tree for buffer\n");
		return XML_ERR_ENC_OVERFLOW;
	}

	if(curNode->children) {
		curNode = curNode->children;
		goto NEW_NODE;
	}

NEXT_NODE:
	if(curNode->next) {
		if(curNode->nodeType == NODE_PARENT) {
			*encPtr += sprintf(*encPtr, "</%s>", curNode->tagField);
		}
		curNode = curNode->next;
		DEBUG_ENC ("3:\n\r%s\n\r", encodedPktStr);
		goto NEW_NODE;
	} else {
		if(curNode->nodeType == NODE_PARENT) {
			*encPtr += sprintf(*encPtr, "</%s>", curNode->tagField);
			DEBUG_ENC ("4:\n\r%s\n\r", encodedPktStr);
		}
		if(curNode->parent) {
			curNode = curNode->parent;
			DEBUG_ENC ("back to: %s\n\r", curNode->tagField);
			goto NEXT_NODE;
		}
	}

	return XML_SUCCESS;
}

xml_status_t encode_xml_pkt(char **xmlData)
{
    xml_status_t ret;
	char *encPtr = NULL;

	if((NULL == rootNode) || \
			IS_INVALID_NODE_TYPE(rootNode->nodeType) || \
			!IS_NODE_SINGLE_VALID(rootNode)) {
		return XML_FAILURE;
	}

	encodedPktStr = malloc(MAX_TX_XML_PKT_SIZE);
	if(!encodedPktStr) {
		printf("Error allocating in XML encoder\n");
		return XML_ERR_ALLOC;
	}

	memset(encodedPktStr, 0, MAX_TX_XML_PKT_SIZE);
	encPtr = encodedPktStr;

	ret = enc_buildXmlBuf(&encPtr);
	if(XML_SUCCESS != ret) {
		printf("failed to build xml packet buffer!\n");
		free(encodedPktStr);
	    destroyXMLtree();
		return XML_FAILURE;
	}

	/*APPLICATION SHOULD FREE THE XML BUFFER AFTER USING IT*/
	*xmlData = encodedPktStr;
	DEBUG_ENC ("PACKET: \n%s\n", encodedPktStr);
	return ret;
}

#if USING_TAG_ATTR
static void destroyAttrList(NODE_ATTR_T **list)
{
	NODE_ATTR_T *cur = *list, *prev;

	do {
		prev = cur;
		free(cur->attrName);
		free(cur->attrValue);
	} while(NULL != (cur = prev->next));
}
#endif

void destroyXMLtree(void)
{
	XML_NODE_T *curNode = rootNode;

	if(!curNode) {
		return;
	}

	DEBUG_ENC ("Destroying xml tree from: %s\r\n", rootNode->tagField);

	rootNode = NULL;
	while(curNode) {
		DEBUG_ENC ("cur: %p:%s\n", curNode, curNode->tagField);
		if(curNode->children) {
			curNode = curNode->children;
		} else {
			XML_NODE_T *temp;
			uint8_t isParent = 0;
FREE_NODE:
			if(curNode->next) {
				temp = curNode->next;
			} else {
				temp = curNode->parent;
				if(temp) {
					isParent = 1;
				}
			}
			DEBUG_ENC ("freeing: %s\n", curNode->tagField);
			free(curNode->tagField);
#if USING_TAG_ATTR
			if(curNode->attrList) {
				destroyAttrList(&(curNode->attrList));
			}
#endif
			DEBUG_ENC ("freeing: %p\r\n", curNode);
			free(curNode);
			if(isParent) { /*all children dead, so parent also dies*/
				curNode = temp;
				temp = temp->next;
				isParent = 0;
				goto FREE_NODE;
			}
			curNode = temp;
		}
	}
	DEBUG_ENC ("destruction of the Tree complete\r\n");
}

XML_NODE_T *findNodeWithTag(char *name, XML_NODE_T *start)
{
	XML_NODE_T *curNode = start;

	if(!start) {
		return (XML_NODE_T *)NULL;
	}

	if(!strcmp(name, start->tagField)) {
		return start;
	}

	while(curNode) {
 	    DEBUG_ENC ("check %s\n", curNode->tagField);
		if(!strcmp(name, curNode->tagField))
			break;
		if(curNode->children) {
			curNode = curNode->children;
		} else { /*try list of children*/
			/*If no more children, goto next node of parent, 
			 *since parent is already checked*/
			curNode = (curNode->next)?curNode->next:curNode->parent->next;
		}
	}

 	DEBUG_ENC ("findNode: %s %s\r\n", name, curNode?"success":"failed");

	return curNode;
}

#if USING_TAG_ATTR
xml_status_t addAttrToNode(char *attrName, attrValue_u *value, attr_type_t type, char *nodeName)
{
#define IS_INVALID_ATTR_TYPE(type) ((type < ATTR_CHAR) || (type > ATTR_MAX))

	DEFINE_FORMAT_SPECS_ARRAY(fmt_spec);
	XML_NODE_T *node;
	NODE_ATTR_T *newAttr;
	char *temp;
	uint8_t n = 0;

	if(!nodeName) {
		printf("null address given for node!\n");
		return XML_ERR_NULL_MEM;
	}
	if(!attrName || !value || IS_INVALID_ATTR_TYPE(type)) {
		printf("No proper attr input\n");
		return XML_ERR_BAD_ATTR;
	}

	node = findNodeWithTag(nodeName, rootNode);
	if(!node) {
		printf("No node named %s\n", nodeName);
		return XML_ERR_BAD_NODE;
	}

	newAttr = calloc(1, sizeof(NODE_ATTR_T));
	if(!newAttr) {
		printf("memory allocation error for new attr\n");
		return XML_ERR_ALLOC;
	}

	newAttr->attrName = xstrndup(attrName, strlen(attrName));
	DEBUG_ENC ("attr: %s=", newAttr->attrName);
	temp = calloc(1, 2 + 20);
	if(!temp) {
		printf("memory allocation error for new attr value\n");
		return XML_ERR_ALLOC;
	}

	DEBUG_ENC ("val: %u\n", value->n_u8);
	n += sprintf(temp + n,fmt_spec[type], value->xVal);
	newAttr->attrValue = xstrndup(temp, strlen(temp));
	free(temp);
	DEBUG_ENC ("%s\n", newAttr->attrValue);

	if(!node->attrList) {
		DEBUG_ENC ("1st attr\n");
		node->attrList = newAttr;
		node->numOfAttr = 0;
	} else {
		NODE_ATTR_T *link = node->attrList;
		DEBUG_ENC ("linking attr\n");
		for(; link->next; link = link->next);
		link->next = newAttr;
	}

	node->numOfAttr++;

	return XML_SUCCESS;
}
#endif

/**
 * @func   : addNodeToTree
 * @brief  : adds a new node to tree
 * @called : when populating tree with a new node as child/next/first
 * @input  : char *new - tagField for tagName/content/cdata/comment
 *           char *addTo - tagField for previous node to which new one is added
 *                       This is required so that user can properly populate nodes
 *           node_type_t nodeType - type of new node
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
xml_status_t addNodeToTree(const char *new, node_type_t nodeType, const char *addTo, xml_add_t addType)
{
	/*give XML_SUCCESS if addtype is valid*/
#define BAD_ADDTYPE_VAL(type) \
	((type <= XML_ADD_MINTYPE) || (type >= XML_ADD_MAXTYPE))
#define INVALID_POPULATING(type) \
	((type != XML_ADD_AS_CHILD) && (type != XML_ADD_AS_NEXT))
#define ROOT_EXISTS() (NULL != rootNode)

	XML_NODE_T *newNode = NULL;
	static XML_NODE_T *prevNode = NULL;
	xml_status_t xStatus = XML_SUCCESS;

	XML_ASSERT_LBL(IS_INVALID_NODE_TYPE(nodeType), XML_ERR_BAD_NODE, "Invalid nodeType\n");

	XML_NULL_ASSERT_LBL(new);

	XML_ASSERT_LBL(BAD_ADDTYPE_VAL(addType), XML_ERR_BAD_NODE, "Invalid addType\n");

	DEBUG_ENC ("newNode: %s\r\n", new);
// 	XML_ASSERT_LBL((INVALID_POPULATING(addType) && ROOT_EXISTS()), XML_ERR_BAD_NODE, "Invalid addType\n");

	newNode = calloc(1, sizeof(XML_NODE_T));
	XML_NULL_ASSERT_LBL(newNode);

	newNode->tagField = xstrndup((char *)new, strlen(new));
	XML_NULL_ASSERT_LBL(newNode->tagField);

	newNode->nodeType = nodeType;

	if(addType == XML_ADD_AS_FIRST) {
		/*destroy any previous tree to have new tree planted*/
	    destroyXMLtree();
		rootNode = prevNode = newNode;
		DEBUG_ENC ("\"%s\" is rootNode\n", rootNode->tagField);
		return XML_SUCCESS;
	} else {
		XML_NULL_ASSERT_LBL(addTo);
	}

	if(XML_SUCCESS != strcmp(addTo, prevNode->tagField)) {
	   	if(XML_SUCCESS == strcmp(addTo, prevNode->parent->tagField)) {
			DEBUG_ENC ("adding %s to a prev parent: %s\n", new, prevNode->parent->tagField);
			prevNode = prevNode->parent;
		} else {
			printf("don't know :%s..%s..%s\n", addTo, new, prevNode->tagField);
			xStatus = XML_ERR_BAD_NODE;
			goto ERR_XML;
		}
	}

	if (addType == XML_ADD_AS_CHILD) {
		if(prevNode->children) {
			printf("%s has already got a child\n", addTo);
			xStatus = XML_ERR_BAD_NODE;
			goto ERR_XML;
		} 
		DEBUG_ENC ("adding %s as child to %s\n", new, addTo);
		prevNode->children = newNode;
		prevNode->numOfChildren = 1;
		newNode->parent = prevNode;
	} else {
		if(prevNode->next) {
			printf("%s has already got a node next to it!\n", prevNode->tagField);
			xStatus = XML_ERR_BAD_NODE;
			goto ERR_XML;
		}
		DEBUG_ENC ("adding as next\n");
		newNode->parent = prevNode->parent;
		prevNode->next = newNode;
		prevNode->parent->numOfChildren++;
	}

	prevNode = newNode;

ERR_XML:
	if(xStatus != XML_SUCCESS) {
		if(newNode->tagField)
			free(newNode->tagField);
		if(newNode)
			free(newNode);
		destroyXMLtree();
		prevNode = NULL;
	}
	DEBUG_ENC ("now prev: %s\n", prevNode->tagField);
	return xStatus;
}
