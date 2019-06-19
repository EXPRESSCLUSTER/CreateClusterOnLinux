#include <stdio.h>
#include <libxml/tree.h>


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


int
main
(
	int argc,
	char *argv[]
)
{
	xmlDocPtr doc;
	xmlNodePtr node;
	char curr[1024];
	char path[1024];
	char element[1024];
	char attribute[1024];
	char *p;
	int ret;

	/* root node */
	strcpy(curr, "/root");

	memset(attribute, 0, sizeof(attribute));

	/* search path */
	strcpy(path, argv[1]);
//	strcpy(path, "/root/webmgr/client/objectnumber");

	doc = xmlParseFile("clp.conf");

//	find_value_node(doc, node, path, curr);	
	for (node = doc->children; node != NULL; node = node->next)
	{
		printf("node->name: %s\n", node->name);
		ret = seek(node, curr, path);
		if (ret)
		{
			printf("Not found\n");
		}
		
	}

//	xmlSaveFormatFile("clp.conf", doc, 0);

	return 0;
}