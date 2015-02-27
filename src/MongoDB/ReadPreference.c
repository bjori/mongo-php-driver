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
#include <mongoc-read-prefs-private.h>

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


PHONGO_API zend_class_entry *php_phongo_readpreference_ce;

zend_object_handlers php_phongo_handler_readpreference;

/* {{{ proto MongoDB\Driver\ReadPreference ReadPreference::__construct(integer $readPreference[, array $tagSets = array()])
   Constructs a new ReadPreference */
PHP_METHOD(ReadPreference, __construct)
{
	php_phongo_readpreference_t *intern;
	long                      readPreference;
	zval                     *tagSets = NULL;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	intern = (php_phongo_readpreference_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|a!", &readPreference, &tagSets) == FAILURE) {
		return;
	}


	switch(readPreference) {
		case MONGOC_READ_PRIMARY:
		case MONGOC_READ_SECONDARY:
		case MONGOC_READ_PRIMARY_PREFERRED:
		case MONGOC_READ_SECONDARY_PREFERRED:
		case MONGOC_READ_NEAREST:
			intern->read_preference = mongoc_read_prefs_new(readPreference);

			if (tagSets) {
				bson_t *tags = bson_new();

				zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t *)tags, NULL TSRMLS_CC);
				mongoc_read_prefs_set_tags(intern->read_preference, tags);
				bson_destroy(tags);
				if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s", "Invalid tagSet");
					return;
				}
			}
			break;
		default:
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s", "Invalid ReadPreference");
			return;
	}
}
/* }}} */

/**
 * Value object for read preferences used in issuing commands and queries.
 */
/* {{{ MongoDB\Driver\ReadPreference */

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, readPreference)
	ZEND_ARG_ARRAY_INFO(0, tagSets, 1)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_readpreference_me[] = {
	PHP_ME(ReadPreference, __construct, ai_ReadPreference___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_readpreference_t object handlers */
static void php_phongo_readpreference_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern = (php_phongo_readpreference_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->read_preference) {
		mongoc_read_prefs_destroy(intern->read_preference);
	}

	efree(intern);
} /* }}} */

zend_object_value php_phongo_readpreference_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_readpreference_t *intern = NULL;

	intern = (php_phongo_readpreference_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_readpreference_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_readpreference;

	return retval;
} /* }}} */

HashTable *php_phongo_readpreference_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zval                 retval = zval_used_for_init;
	mongoc_read_prefs_t *read_prefs = phongo_read_preference_from_zval(object TSRMLS_CC);


	*is_temp = 1;

	php_phongo_read_preference_to_zval(&retval, read_prefs);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ReadPreference)
{
	(void)type; (void)module_number;
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadPreference", php_phongo_readpreference_me);
	php_phongo_readpreference_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_readpreference_ce->create_object = php_phongo_readpreference_create_object;
	php_phongo_readpreference_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	memcpy(&php_phongo_handler_readpreference, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readpreference.get_debug_info = php_phongo_readpreference_get_debug_info;

	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY"), MONGOC_READ_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY_PREFERRED"), MONGOC_READ_PRIMARY_PREFERRED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY"), MONGOC_READ_SECONDARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY_PREFERRED"), MONGOC_READ_SECONDARY_PREFERRED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_NEAREST"), MONGOC_READ_NEAREST TSRMLS_CC);

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
