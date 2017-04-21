
#ifndef __XML_H
#define __XML_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>

/* define as 1 if debug prints are needed in xml_encoder.c */
#define ENABLE_ENC_DBG 0
/* define as 1 if debug prints are needed in xml_decoder.c */
#define ENABLE_DEC_DBG 0

/* if error prints not needed, define as 0 */
#define XML_ERR_PRINTS_ALLOWED 1

#if XML_ERR_PRINTS_ALLOWED
#define XML_ASSERT_LBL(condition, errCode, err) \
	do { if(condition) { printf(err" @ %s +%u\r\n", __func__, __LINE__);  xStatus = errCode; goto ERR_XML; } } while(0)
#else
#define XML_ASSERT_LBL(condition, errCode, err) \
	do { if(condition) { xStatus = errCode; goto ERR_XML; } } while(0)
#endif

#define XML_FUNC_ASSERT_LBL(function, err) \
	XML_ASSERT_LBL((XML_SUCCESS != (xStatus = function)), xStatus, err)

#define XML_NULL_ASSERT_LBL(ptr) \
	XML_ASSERT_LBL((NULL == ptr), XML_ERR_ALLOC, "Null Buffer: "#ptr)

/*xml packet size limits*/
#define MAX_TX_XML_PKT_SIZE 2048
#define MAX_RX_XML_PKT_SIZE 2048

#define XML_INSTR_STR       "xml version=\"1.0\""

/*define USING_xxx as 1 if the feature xxx should be included*/
#define USING_COMMENTS 0 
#define USING_CDATA    1
#define USING_INSTR    1
#define USING_TAG_ATTR 0

/*can be removed as required.
 *see func addNodeToTree from xml_encoder.c*/
#define STRICT 1

#pragma pack(1)

typedef enum {
	XML_SUCCESS = 0,
	XML_ERR_ENC_OVERFLOW,
	XML_ERR_DEC_OVERFLOW,
	XML_ERR_ALLOC,
	XML_ERR_NULL_MEM,
	XML_ERR_BAD_NODE,
	XML_ERR_BAD_TAG,
#if USING_TAG_ATTR
	XML_ERR_BAD_ATTR,
#endif
	XML_ERR_BAD_NODETYPE,
	XML_FAILURE,
} xml_status_t;

typedef enum {
	NODE_MIN = 0,
	NODE_PARENT,
	NODE_SINGLE,
#if USING_COMMENTS
	NODE_COMMENT,
#endif
#if USING_INSTR
	NODE_INSTR,
#endif
#if USING_CDATA
	NODE_CDATA,
#endif
	NODE_CONTENT,
	NODE_MAX,
} node_type_t;

#if USING_TAG_ATTR
typedef enum {
	ATTR_CHAR,
	ATTR_STR,
	ATTR_UINT8,
	ATTR_INT8,
	ATTR_UINT16,
	ATTR_INT16,
	ATTR_UINT32,
	ATTR_INT32,
	ATTR_FLOAT,
	ATTR_MAX,
} attr_type_t;

/*Modifying this requires modifying above enum for data types;
 * ensure proper format specifiers while encoding different data in xml*/
#define DEFINE_FORMAT_SPECS_ARRAY(arr) \
	char arr[ATTR_MAX][4] = {"%c", "%s", "%u", "%d", "%hu", "%hd", "%u", "%d", "%f"};
#endif

#define NAME2STR(x) (#x)

typedef enum {
	XML_ADD_MINTYPE,
	XML_ADD_AS_CHILD,
	XML_ADD_AS_NEXT,
	XML_ADD_AS_FIRST,
	XML_ADD_MAXTYPE,
} xml_add_t;

#if USING_TAG_ATTR
typedef struct node_attr_struct {
	char                    *attrName;
	char                    *attrValue;
	struct node_attr_struct *next;
} NODE_ATTR_T;
#endif

typedef struct xml_node_struct {
	uint8_t                nodeType;
	char                   *tagField;
	struct xml_node_struct *parent;
#if USING_TAG_ATTR
	uint8_t                numOfAttr;
	NODE_ATTR_T            *attrList;
#endif
	uint8_t                numOfChildren;
	struct xml_node_struct *children;
	struct xml_node_struct *next;
} __attribute__((__packed__)) XML_NODE_T;

xml_status_t encode_xml_pkt(char **xmlData);
xml_status_t decode_xml_pkt(char *str, XML_NODE_T **xmlNode);

#if USING_TAG_ATTR
typedef union attrValue_union {
	uint32_t  xVal; /*to refer all variables here in generic*/
	uint32_t  n_u32;
    int32_t   n_i32;
    uint16_t  n_u16;
    int16_t   n_i16;
    int8_t    n_i8;
    uint8_t   n_u8;
    float     f;
    char      *str;
    char      ch;
} attrValue_u;
#endif

char *xstrndup(char *src, size_t len);

/**
 * @func   : addFirstNode
 * @brief  : adds first(root?) node to tree
 * @called : when populating tree for the first time
 * @input  : char *name - tagField
 *           node_type_t nodeType - type of node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addFirstNode(name, nodeType) \
		addNodeToTree(name, nodeType, NULL, XML_ADD_AS_FIRST)

/**
 * @func   : addParentNode
 * @brief  : adds PARENT type node to tree
 * @called : when populating tree with a PARENT type node
 * @input  : char *name - tagField
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addParentNode(name, addTo, addType) \
		addNodeToTree(name, NODE_PARENT, addTo, addType)

/**
 * @func   : addSingleNode
 * @brief  : adds SINGLE type node to tree
 * @called : when populating tree with a SINGLE type node (tag is '<node/>')
 * @input  : char *name - tagField
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addSingleNode(name, addTo, addType) \
		addNodeToTree(name, NODE_SINGLE, addTo, addType)

#if USING_COMMENTS
/**
 * @func   : addCommentNode
 * @brief  : adds COMMENT type node to tree
 * @called : when populating tree with a COMMENT type node: <!-- comment -->
 * @input  : char *comment - comment string
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addCommentNode(comment, addTo, addType) \
		addNodeToTree(comment, NODE_COMMENT, addTo, addType)
#endif

#if USING_CDATA
/**
 * @func   : addCDataNode
 * @brief  : adds CDATA type node to tree
 * @called : when populating tree with a CDATA type node: <![CDATA[ cdata-string ]]>
 * @input  : char *CData - CDATA string
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addCDataNode(CData, addTo, addType) \
		addNodeToTree(CData, NODE_CDATA, addTo, addType)
#endif

#if USING_INSTR
/**
 * @func   : addInstrNode
 * @brief  : adds INSTRUCTION type node to tree: <?instr-string?>
 * @called : when populating tree with a INSTRUCTION type node
 * @input  : char *instr - instruction string
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addInstrNode(instr, addTo, addType) \
		addNodeToTree(instr, NODE_INSTR, addTo, addType)
#endif

/**
 * @func   : addContentNode
 * @brief  : adds CONTENT type node to tree
 * @called : when populating tree with a CONTENT type node(just data without tag)
 * @input  : char * content - content string
 *           char *addTo - name of node to which current node is added
 *           xml_add_t addType - to add as child/next/first node
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_NODETYPE/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_ERR_BAD_NODE/XML_FAILURE
 */
#define addContentNode(content, addTo, addType) \
		addNodeToTree(content, NODE_CONTENT, addTo, addType)

xml_status_t addNodeToTree(const char *tagField, node_type_t nodeType, const char *addTo, xml_add_t addType);

#if USING_TAG_ATTR
/**
 * @func   : addAttr_CHAR
 * @brief  : adds CHAR type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_CHAR(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_CHAR, node)

/**
 * @func   : addAttr_STR
 * @brief  : adds STRING type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_STR(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_STR, node)

/**
 * @func   : addAttr_UINT8
 * @brief  : adds unsigend 8-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_UINT8(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_UINT8, node)

/**
 * @func   : addAttr_INT8
 * @brief  : adds signed 8-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_INT8(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_INT8, node)

/**
 * @func   : addAttr_UINT16
 * @brief  : adds unsigned 16-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_UINT16(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_UINT16, node)

/**
 * @func   : addAttr_INT16
 * @brief  : adds signed 16-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_INT16(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_INT16, node)

/**
 * @func   : addAttr_UINT32
 * @brief  : adds unsigned 32-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_UINT32(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_UINT32, node)

/**
 * @func   : addAttr_INT32
 * @brief  : adds signed 32-bit int type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_INT32(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_INT32, node)

/**
 * @func   : addAttr_FLOAT
 * @brief  : adds float type attribute to attr list of given node
 * @called : when required to add attr to a node
 * @input  : attrName - name of attr (converted to name string)
 *           attrValue_u attrValue - union with values of any pre-defined datatype
 *           char *node - tagField of node the attr is added to
 * @output : none
 * @return : XML_SUCCESS/XML_ERR_BAD_ATTR/XML_ERR_NULL_MEM/XML_ERR_ALLOC/XML_FAILURE
 */
#define addAttr_FLOAT(attrName, attrValue, node) \
	addAttrToNode(attrName, &attrValue, ATTR_FLOAT, node)

xml_status_t addAttrToNode(char *attrName, attrValue_u *value, attr_type_t type, char *node);
#endif

XML_NODE_T *findNodeWithTag(char *name, XML_NODE_T *start);

void destroyXMLtree(void);
#endif /*__XML_H*/
