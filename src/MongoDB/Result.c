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


PHONGO_API zend_class_entry *php_phongo_result_ce;

zend_object_handlers php_phongo_handler_result;

/* {{{ proto MongoDB\Driver\Result Result::__construct(MongoDB\Driver\Server $server, array|object $responseDocument)
   Constructs a new Result */
PHP_METHOD(Result, __construct)
{
	php_phongo_result_t *intern;
	zval                     *server;
	zval                     *responseDocument;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OA", &server, php_phongo_server_ce, &responseDocument) == FAILURE) {
		return;
	}

}
/* }}} */
/* {{{ proto void Result::setTypemap(array $typemap)
   Sets a typemap to use for BSON unserialization */
PHP_METHOD(Result, setTypemap)
{
	php_phongo_result_t *intern;
	php_phongo_bson_state     state = PHONGO_BSON_STATE_INITIALIZER;
	zval                     *typemap = NULL;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a!", &typemap) == FAILURE) {
		return;
	}

	php_phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC);

	intern->visitor_data = state;
}
/* }}} */
/* {{{ proto MongoDB\Driver\Cursor Result::getIterator()
   Returns the Cursor iterator */
PHP_METHOD(Result, getIterator)
{
	php_phongo_result_t *intern;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (intern->ce_get_iterator) {
		object_init_ex(return_value, intern->ce_get_iterator);
	} else {
		php_phongo_cursor_new_from_result(return_value, intern TSRMLS_CC);
	}

}
/* }}} */
/* {{{ proto self Result::setIteratorClass(string $class)
   Sets the class name of the Cursor implementation to be used */
PHP_METHOD(Result, setIteratorClass)
{
	php_phongo_result_t *intern;
	char                     *class;
	int                       class_len;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &class, &class_len) == FAILURE) {
		return;
	}

}
/* }}} */
/* {{{ proto self Result::setIteratorInitCallback(callable $callback)
   Sets a callback to invoke for initializing a custom Cursor */
PHP_METHOD(Result, setIteratorInitCallback)
{
	php_phongo_result_t *intern;
	zval                     *callback;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &callback) == FAILURE) {
		return;
	}

}
/* }}} */
/* {{{ proto array Result::toArray()
   Returns the original response document from the server */
PHP_METHOD(Result, toArray)
{
	php_phongo_result_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (intern->firstBatch) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(intern->firstBatch), intern->firstBatch->len, &state);
		RETURN_ZVAL(state.zchild, 0, 1);
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server Result::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(Result, getServer)
{
	php_phongo_result_t *intern;
	mongoc_host_list_t        host;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_result_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	mongoc_cursor_get_host(intern->cursor, &host);
	phongo_server_init(return_value, intern->hint, &host TSRMLS_CC);
}
/* }}} */

/* {{{ MongoDB\Driver\Result */

ZEND_BEGIN_ARG_INFO_EX(ai_Result___construct, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, server, MongoDB\\Driver\\Server, 0)
	ZEND_ARG_INFO(0, responseDocument)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_setTypemap, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, typemap, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_getIterator, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_setIteratorClass, 0, 0, 1)
	ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_setIteratorInitCallback, 0, 0, 1)
	ZEND_ARG_INFO(0, callback) /* callable */
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_toArray, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Result_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_result_me[] = {
	PHP_ME(Result, __construct, ai_Result___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, setTypemap, ai_Result_setTypemap, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, getIterator, ai_Result_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, setIteratorClass, ai_Result_setIteratorClass, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, setIteratorInitCallback, ai_Result_setIteratorInitCallback, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, toArray, ai_Result_toArray, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Result, getServer, ai_Result_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_result_t object handlers */
static void php_phongo_result_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_result_t *intern = (php_phongo_result_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	php_phongo_result_free(intern);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_result_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value           retval;
	php_phongo_result_t *intern = NULL;

	intern = (php_phongo_result_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_result_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_result;

	return retval;
} /* }}} */

HashTable *php_phongo_result_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_result_t    *intern;
	zval                           retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_result_t *)zend_object_store_get_object(object TSRMLS_CC);

	php_phongo_result_to_zval(&retval, intern);

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Result)
{
	(void)type; (void)module_number;
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Result", php_phongo_result_me);
	php_phongo_result_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_result_ce->create_object = php_phongo_result_create_object;
	php_phongo_result_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;
	php_phongo_result_ce->get_iterator = phongo_result_get_iterator;

	memcpy(&php_phongo_handler_result, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_result.get_debug_info = php_phongo_result_get_debug_info;

	zend_class_implements(php_phongo_result_ce TSRMLS_CC, 1, zend_ce_aggregate);


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
