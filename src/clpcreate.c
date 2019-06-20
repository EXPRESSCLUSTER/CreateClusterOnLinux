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
		else if (!strcmp(argv[2], "objnum"))
		{
			add_obj_num(argv[3]);
		}
	}
	else
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
	}

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
	xmlDocPtr doc;

	doc = xmlParseFile("clp.conf");

	set_value(doc, "/root/all/charset", lang);
	set_value(doc, "/root/all/serveros", os);
	set_value(doc, "/root/all/encode", lang);
	set_value(doc, "/root/cluster/name", clsname);
	set_value(doc, "/root/cluster/comment", "");

	xmlSaveFormatFileEnc("clp.conf", doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

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
	xmlDocPtr	doc;
	char 		path[1024];

	doc = xmlParseFile("clp.conf");
	
	strcpy(path, "/root/server@");
	strcat(path, srvname);
	strcat(path, "/priority");
	set_value(doc, path, priority);

	xmlSaveFormatFileEnc("clp.conf", doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

func_exit:
	return 0;
}


int
add_obj_num
(
	char *objnum
)
{
	xmlDocPtr	doc;
	char 		path[1024];

	doc = xmlParseFile("clp.conf");
	
	set_value(doc, "/root/webmgr/client/objectnumber", objnum);

	xmlSaveFormatFileEnc("clp.conf", doc, "UTF-8", 1);

	xmlFreeDoc(doc);
	xmlCleanupParser();

func_exit:
	return 0;
}