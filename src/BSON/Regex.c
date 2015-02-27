/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_regex_ce;

/* {{{ proto BSON\Regex Regex::__construct(string $pattern, string $flags)
   Constructs a new regular expression. */
PHP_METHOD(Regex, __construct)
{
	php_phongo_regex_t       *intern;
	char                     *pattern;
	int                       pattern_len;
	char                     *flags;
	int                       flags_len;


	intern = (php_phongo_regex_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &pattern, &pattern_len, &flags, &flags_len) == FAILURE) {
		return;
	}


	intern->pattern = estrndup(pattern, pattern_len);
	intern->pattern_len = pattern_len;
	intern->flags = estrndup(flags, flags_len);
	intern->flags_len = flags_len;
}
/* }}} */
/* {{{ proto void Regex::getPattern()
    */
PHP_METHOD(Regex, getPattern)
{
	php_phongo_regex_t       *intern;


	intern = (php_phongo_regex_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_STRINGL(intern->pattern, intern->pattern_len, 1);
}
/* }}} */
/* {{{ proto void Regex::getFlags()
    */
PHP_METHOD(Regex, getFlags)
{
	php_phongo_regex_t       *intern;


	intern = (php_phongo_regex_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_STRINGL(intern->flags, intern->flags_len, 1);
}
/* }}} */
/* {{{ proto void Regex::__toString()
    */
PHP_METHOD(Regex, __toString)
{
	php_phongo_regex_t       *intern;
	char                     *regex;
	int                       regex_len;


	intern = (php_phongo_regex_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	regex_len = spprintf(&regex, 0, "/%s/%s", intern->pattern, intern->flags);
	RETVAL_STRINGL(regex, regex_len, 0);
}
/* }}} */


/* {{{ BSON\Regex */

ZEND_BEGIN_ARG_INFO_EX(ai_Regex___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Regex_getPattern, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Regex_getFlags, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Regex___toString, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_regex_me[] = {
	PHP_ME(Regex, __construct, ai_Regex___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Regex, getPattern, ai_Regex_getPattern, ZEND_ACC_PUBLIC)
	PHP_ME(Regex, getFlags, ai_Regex_getFlags, ZEND_ACC_PUBLIC)
	PHP_ME(Regex, __toString, ai_Regex___toString, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_regex_t object handlers */
static void php_phongo_regex_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t *intern = (php_phongo_regex_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

    if (intern->pattern) {
        efree(intern->pattern);
    }

    if (intern->flags) {
        efree(intern->flags);
    }
	efree(intern);
} /* }}} */

zend_object_value php_phongo_regex_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_regex_t *intern;

	intern = (php_phongo_regex_t *)emalloc(sizeof(php_phongo_regex_t));
	memset(intern, 0, sizeof(php_phongo_regex_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_regex_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Regex)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "BSON", "Regex", php_phongo_regex_me);
	ce.create_object = php_phongo_regex_create_object;
	php_phongo_regex_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(php_phongo_regex_ce TSRMLS_CC, 1, php_phongo_type_ce);


	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
