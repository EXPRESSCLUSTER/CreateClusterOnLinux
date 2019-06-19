#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "clpcreate.h"


int
init
(
	void
)
{
	xmlDocPtr	doc;
	xmlNodePtr	root_node = NULL;
	char		output_file[1024];

	strcpy(output_file, "clp.conf");

	doc = xmlNewDoc("1.0");
	root_node = xmlNewNode(NULL, "root");
	xmlDocSetRootElement(doc, root_node);
	xmlSaveFormatFileEnc(output_file, doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}


xmlDocPtr
read_xml
(
	char *filename
)
{
	xmlDocPtr doc;
	doc = xmlParseFile(filename);

	return doc;
}


xmlNodePtr
add_node
(
	xmlNodePtr node,
	char *node_name,
	char *text,
	char *attr,
	char *attr_var
)
{
	xmlNodePtr new_node = NULL;
	xmlNodePtr new_text = NULL;

	new_node = xmlNewNode(NULL, node_name);

	if (text != NULL)
	{
		new_text = xmlNewText(text);
		xmlAddChild(new_node, new_text);
	}
	
	if (attr != NULL)
	{
		xmlNewProp(new_node, attr, attr_var);
	}

	xmlAddChild(node, new_node);
}


int
seek
(
	xmlNodePtr node,
	char *curr,
	char *path
)
{
	char element[1024];
	char *p;
	int ret;

	printf("curr: %s\n", curr);
	printf("path: %s\n", path);
	if (!strcmp(curr, path))
	{
		return 0;
	}

	strcpy(element, &path[strlen(curr) + 1]);
	
	p = strchr(element, '/');
	if (p != NULL)
	{
		*p = '\0';
	}
	
	p = strchr(element, '@');
	if (p != NULL)
	{
		*p = '\0';
//		strcpy(attribute, ++p);
	}
	printf("element: %s\n", element);

	for (node = node->children; node != NULL; node = node->next)
	{
		printf("node->name: %s\n", node->name);
		if (!strcmp(node->name, element))
		{
			printf("Bingo!!\n");
			strcat(curr, "/");
			strcat(curr, element);
			ret = seek(node, curr, path);
			if (!ret)
			{
				return 0;
			}
		}
	}
	return 1;
}

int
find_child_node
(
	xmlDocPtr doc,
	xmlNodePtr node,
	char *element,
	char *attribute
)
{
	printf("element: %s\n", element);
	
//	for (node = doc->children; node != NULL; node = node->next)
	for (node = doc->children; node != NULL; node = node->next)
	{
		printf("node->name: %s\n", node->name);
//		seek(node);
	}

	return 0;
}


int
find_value_node
(
	xmlDocPtr doc,
	xmlNodePtr node,
	char *path, 
	char *curr
)
{
	char element[1024];
	char attribute[1024];
	char *p;

	int ret;

	printf("path: %s\n", path);
	printf("curr: %s\n", curr);

	strcpy(element, &path[strlen(curr) + 1]);
	strcpy(attribute, "");
	
	p = strchr(element, '/');
	if (p != NULL)
	{
		*p = '\0';
	}
	
	p = strchr(element, '@');
	if (p != NULL)
	{
		*p = '\0';
		strcpy(attribute, ++p);
	}

	ret = find_child_node(doc, node, element, attribute);
	if (ret != 0)
	{
		
	}
	/* continue? */
	strcat(curr, "/");
	strcat(curr, element);

	if (strlen(attribute) > 0)
	{
		strcat(curr, "@");
		strcat(curr, attribute);
	}

	if (strcmp(path, curr) != 0)
	{
//		ret = find_value_node(doc, node->children, path, curr);
	}
	
}