#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlstring.h>

#include "clpcreate.h"

int child_hit_flag;


int
init(
	char *encoding
)
{
	xmlDocPtr	doc;
	xmlNodePtr	root_node = NULL;
	char		output_file[1024];

	strcpy(output_file, "clp.conf");

	doc = xmlNewDoc("1.0");
	root_node = xmlNewNode(NULL, "root");
	xmlDocSetRootElement(doc, root_node);
	xmlSaveFormatFileEnc(output_file, doc, encoding, 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}


int
set_value(
	xmlDocPtr	doc,
	char		*path,
	char		*value
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	target_node = NULL;
	xmlNodePtr	new_text = NULL;
	char		curr[1024];
	int			ret = 0;

	strcpy(curr, "/root");
	root_node = xmlDocGetRootElement(doc);
	target_node = find_value_node(doc, root_node, path, curr);

	if (target_node != NULL && value != NULL)
	{
		new_text = xmlNewText(value);
		xmlAddChild(target_node, new_text);
	}

	return ret;
}


xmlNodePtr
find_child_node(
	xmlDocPtr	doc,
	xmlNodePtr	node,
	char		*element,
	char		*attribute
)
{
	xmlNodePtr ret_node = NULL;
	xmlNodePtr n;
	
	for (n = node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			if (strlen(attribute) > 0)
			{
				if (strcmp(xmlGetProp(n, n->properties->name), attribute) != 0)
				{
					continue;
				}
			}

			ret_node = n;
			break;
		}
	}

	return ret_node;
}


xmlNodePtr
find_value_node(
	xmlDocPtr	doc,
	xmlNodePtr	node,
	char		*path, 
	char		*curr
)
{
	xmlNodePtr		ret_node;
	char			element[1024];
	char			attribute[1024];
	char			*p;
	int				ret;

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

	child_hit_flag = 0;
	ret_node = find_child_node(doc, node, element, attribute);
	if (ret_node == NULL)
	{
		ret_node = make_child_node(node, element, attribute);
		child_hit_flag = 0;
	}
	else
	{
		child_hit_flag = 1;
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
		ret_node = find_value_node(doc, ret_node, path, curr);
	}
	
func_exit:

	if (child_hit_flag == 0)
	{
		return ret_node;
	}
	else
	{
		return NULL;
	}
}


xmlNodePtr
make_child_node(
	xmlNodePtr	node,
	char		*node_name,
	char		*attr_var
)
{
	xmlNodePtr	new_node = NULL;
	xmlNodePtr	ret_node;
	int			ret = 0;
	char		attr_name[1024];

	new_node = xmlNewNode(NULL, node_name);

	if (strlen(attr_var) > 0)
	{
		if (!strcmp(node_name, "device") | !strcmp(node_name, "list") | !strcmp(node_name, "hba") | !strcmp(node_name, "grp"))
		{
			strcpy(attr_name, "id");
		}
		else
		{
			strcpy(attr_name, "name");
		}

		xmlNewProp(new_node, attr_name, attr_var);
	}

	ret_node = xmlAddChild(node, new_node);
	return ret_node;
}