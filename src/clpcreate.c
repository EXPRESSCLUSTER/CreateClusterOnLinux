#include <stdio.h>
#include <libxml/tree.h>
//#include <libxml/xpath.h>
#include <libxml/parser.h>
//#include <libxml/xpathInternals.h>


xmlNodePtr
add_node(
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
main(
	int argc,
	char **argv
)
{
	xmlDocPtr	doc;
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	new_node = NULL;
	char		*output_file;
	int		ret;

	if (argc != 2)
	{
		return 1;
	}
	output_file = argv[1];

	doc = xmlNewDoc("1.0");
	root_node = xmlNewNode(NULL, "root");
	xmlDocSetRootElement(doc, root_node);

	new_node = add_node(root_node, "cluster", "mytest", NULL, NULL);
//	add_node(root_node, "cluster/comment", " ", NULL, NULL);

	xmlSaveFormatFileEnc(output_file, doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}
