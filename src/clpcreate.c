#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
//#include <libxml/xpath.h>
#include <libxml/parser.h>
//#include <libxml/xpathInternals.h>

#include "clpcreate.h"


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
	int			ret;

	if (!strcmp(argv[1], "init"))
	{
		init();
		goto func_exit;
	}

	

	if (!strcmp(argv[1], "add"))
	{
		if (!strcmp(argv[2], "cls"))
		{
			add_cls(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "srv"))
		{
			add_srv(argv[3], argv[4]);
		}
	}
	else
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
	}
/* 
	new_node = add_node(root_node, "cluster", "mytest", NULL, NULL);
	new_node = add_node(root_node, "server", "server1", NULL, NULL);
	new_node = add_node(root_node, "server", "", NULL, NULL);
	new_node = add_node(root_node, "resource", NULL, NULL, NULL);
	new_node = add_node(new_node, "fip", "fip1", "attr", "test_attr");

	xmlSaveFormatFileEnc(output_file, doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();
*/
func_exit:
	return 0;
}


int
add_cls
(
	char *clsname,
	char *lang,
	char *os
)
{

	func_exit:
	return 0;
}


int
add_srv
(
	char *srvname,
	char *priority
)
{

	func_exit:
	return 0;
}