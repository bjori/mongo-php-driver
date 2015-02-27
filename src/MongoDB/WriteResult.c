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


PHONGO_API zend_class_entry *php_phongo_writeresult_ce;

zend_object_handlers php_phongo_handler_writeresult;

/* {{{ proto integer WriteResult::getInsertedCount()
   Returns the number of documents that were inserted */
PHP_METHOD(WriteResult, getInsertedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->nInserted);
}
/* }}} */
/* {{{ proto integer WriteResult::getMatchedCount()
   Returns the number of documents that matched the update criteria */
PHP_METHOD(WriteResult, getMatchedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->nMatched);
}
/* }}} */
/* {{{ proto integer WriteResult::getModifiedCount()
   Returns the number of documents that were actually modified by an update */
PHP_METHOD(WriteResult, getModifiedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->nModified);
}
/* }}} */
/* {{{ proto integer WriteResult::getDeletedCount()
   Returns the number of documents that were deleted */
PHP_METHOD(WriteResult, getDeletedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->nRemoved);
}
/* }}} */
/* {{{ proto integer WriteResult::getUpsertedCount()
   Returns the number of documents that were upserted */
PHP_METHOD(WriteResult, getUpsertedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->nUpserted);
}
/* }}} */
/* {{{ proto array WriteResult::getInfo()
   FIXME: See PHPC-72 & CDRIVER-428
   Returns metadata about the operation. */
PHP_METHOD(WriteResult, getInfo)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (intern->info && Z_TYPE_P(intern->info) == IS_ARRAY) {
		RETURN_ZVAL(intern->info, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server WriteResult::getServer()
   Returns the Server from which the result originated */
PHP_METHOD(WriteResult, getServer)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	phongo_server_init(return_value, intern->result.hint, NULL TSRMLS_CC);
}
/* }}} */
/* {{{ proto array WriteResult::getUpsertedIds()
   Returns the identifiers generated by the server for upsert operations. */
PHP_METHOD(WriteResult, getUpsertedIds)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (intern->upsertedIds && Z_TYPE_P(intern->upsertedIds) == IS_ARRAY) {
		RETURN_ZVAL(intern->upsertedIds, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto WriteConcernError[] WriteResult::getwriteConcernError()
   Return any write concern error that occurred */
PHP_METHOD(WriteResult, getwriteConcernError)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (intern->writeConcernError && Z_TYPE_P(intern->writeConcernError) == IS_OBJECT) {
		RETURN_ZVAL(intern->writeConcernError, 1, 0);
	}

	RETURN_FALSE;
}
/* }}} */
/* {{{ proto WriteError[] WriteResult::getWriteErrors()
   Returns any write errors that occurred */
PHP_METHOD(WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (intern->writeErrors && Z_TYPE_P(intern->writeErrors) == IS_ARRAY) {
		RETURN_ZVAL(intern->writeErrors, 1, 0);
	}

	array_init(return_value);
}
/* }}} */

/**
 * Result returned by Server and Manager executeBulkWrite() methods.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
/* {{{ MongoDB\Driver\WriteResult */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getInsertedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getMatchedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getModifiedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getDeletedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getUpsertedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getUpsertedIds, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getwriteConcernError, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteErrors, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeresult_me[] = {
	PHP_ME(WriteResult, getInsertedCount, ai_WriteResult_getInsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getMatchedCount, ai_WriteResult_getMatchedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getModifiedCount, ai_WriteResult_getModifiedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getDeletedCount, ai_WriteResult_getDeletedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedCount, ai_WriteResult_getUpsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getInfo, ai_WriteResult_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getServer, ai_WriteResult_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedIds, ai_WriteResult_getUpsertedIds, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getwriteConcernError, ai_WriteResult_getwriteConcernError, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteErrors, ai_WriteResult_getWriteErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeresult_t object handlers */
static void php_phongo_writeresult_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern = (php_phongo_writeresult_t*)object;

	zend_object_std_dtor(&intern->result.std TSRMLS_CC);

	if (intern->info) {
		zval_ptr_dtor(&intern->info);
	}

	if (intern->upsertedIds) {
		zval_ptr_dtor(&intern->upsertedIds);
	}

	if (intern->writeConcernError) {
		zval_ptr_dtor(&intern->writeConcernError);
	}

	if (intern->writeErrors) {
		zval_ptr_dtor(&intern->writeErrors);
	}

	php_phongo_result_free(&intern->result);
	efree(intern);
} /* }}} */

zend_object_value php_phongo_writeresult_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writeresult_t *intern = NULL;

	intern = (php_phongo_writeresult_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->result.std, class_type TSRMLS_CC);
	object_properties_init(&intern->result.std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_writeresult;

	return retval;
} /* }}} */

HashTable *php_phongo_writeresult_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern;
	zval                      retval = zval_used_for_init;

	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(object TSRMLS_CC);
	*is_temp = 1;
	array_init_size(&retval, 9);

	{
		zval *result = NULL;

		MAKE_STD_ZVAL(result);
		php_phongo_result_to_zval(result, &intern->result);
		add_assoc_zval_ex(&retval, ZEND_STRS("result"), result);
	}

	add_assoc_long_ex(&retval, ZEND_STRS("nInserted"), intern->nInserted);
	add_assoc_long_ex(&retval, ZEND_STRS("nMatched"), intern->nMatched);
	add_assoc_long_ex(&retval, ZEND_STRS("nModified"), intern->nModified);
	add_assoc_long_ex(&retval, ZEND_STRS("nRemoved"), intern->nRemoved);
	add_assoc_long_ex(&retval, ZEND_STRS("nUpserted"), intern->nUpserted);

	if (intern->info) {
		zval_add_ref(&intern->info);
		add_assoc_zval_ex(&retval, ZEND_STRS("info"), intern->info);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("info"));
	}

	if (intern->upsertedIds) {
		zval_add_ref(&intern->upsertedIds);
		add_assoc_zval_ex(&retval, ZEND_STRS("upsertedIds"), intern->upsertedIds);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("upsertedIds"));
	}

	if (intern->writeErrors) {
		zval_add_ref(&intern->writeErrors);
		add_assoc_zval_ex(&retval, ZEND_STRS("writeErrors"), intern->writeErrors);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("writeErrors"));
	}

	if (intern->writeConcernError) {
		zval_add_ref(&intern->writeConcernError);
		add_assoc_zval_ex(&retval, ZEND_STRS("writeConcernError"), intern->writeConcernError);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("writeConcernError"));
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteResult", php_phongo_writeresult_me);
	php_phongo_writeresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeresult_ce->create_object = php_phongo_writeresult_create_object;
	php_phongo_writeresult_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	memcpy(&php_phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeresult.get_debug_info = php_phongo_writeresult_get_debug_info;

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
