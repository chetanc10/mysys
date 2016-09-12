#include <stdio.h>
#include "xml.h"
#include <stdarg.h>
#include <stdlib.h>

void sampleEncoding(void)
{
	XML_NODE_T *root;
	char *encPtr = NULL;
// 	attrValue_u attrV = {0};

	addFirstNode("root", NODE_PARENT);
// 	attrV.n_u8 = 121;
// 	addAttr_UINT8(ra1, attrV, root);
// 	attrV.str = "jambalakidipamba";
// 	addAttr_STR(ra2, attrV, root);
	addParentNode("child1", "root", XML_ADD_AS_CHILD);
// 	attrV.n_i16 = 21241;
// 	addAttr_INT16(ch1a1, attrV, ch1);
	addContentNode("content to child1!", "child1", XML_ADD_AS_CHILD);
	addSingleNode("grandchild1", "content to child1!", XML_ADD_AS_NEXT);
	addContentNode("content", "grandchild1", XML_ADD_AS_NEXT);
// 	attrV.n_u32 = 41437621;
// 	addAttr_UINT32(sca1, attrV, sc1);

	if(XML_SUCCESS != encode_xml_pkt(&encPtr))
		printf("FAILED\n\r");
	else {
		char c;
		printf("XML PARSED PKT:\n\r%s\n\r", encPtr);
REDO:
		printf("press 'y' to decode encoded pkt: ");
		scanf(" %c", &c);
		if(c == 'y') {
			XML_NODE_T *node;
			if(decode_xml_pkt(encPtr, &node)) {
				printf("FUNN\n");
				destroyXMLtree();
			}
			free(encPtr);
			if(encode_xml_pkt(&encPtr))
				printf("faile encoded deco enc\n");
			else {
				printf("XML PARSED PKT:\n\r%s\n\r", encPtr);
				goto REDO;
			}
		}
	}

	destroyXMLtree();
}

void sampleDecoding(void)
{
	XML_NODE_T *node;

	char *in = "<Root> rootData <Child1> child1data <subC1/>	</Child1> <Child2> child2data </Child2></Root>";
// 	char *in = "<Root rootAttr1=\"1.234\" rootAttr2=\"2.1246\"> rootData <!-- rootComment1 --> <Child1  c11=\"11\"> child1data <subC1  s1=\"12345.824\"/>	</Child1> <Child2 c21=\"21\"> child2data </Child2></Root>";
// 	char *in = "<Root rootAttr1=\"1.234\" rootAttr2=\"2.1246\"> rootData <!-- rootComment1 --> <Child1 c11=\"11\"> </Child1> <Child2> child2data </Child2> </Root>";

	char *str = in;
REDO:
	if(decode_xml_pkt(str, &node)) {
		printf("FUNN\n\r");
	} else {
		char c;
		char *encPtr = NULL;
		printf("FUN\n\r");
		if(encode_xml_pkt(&encPtr)) {
			printf("ENC FUNN\n");
		} else {
			printf("XML PARSED PKT:\n\r%s\n\r", encPtr);
		}
		printf("y for redoing: ");
		scanf(" %c", &c);
		if('y' == c) {
			str = encPtr;
			goto REDO;
		}
	}
}

int main(uint8_t argc, char **argv)
{
	if(argc < 2) {
		printf("Jaffa! give an arg\n\r");
		return;
	}

	if(argv[1][0] == 'e') {
		sampleEncoding();
	} else {
		sampleDecoding();
	}
}
