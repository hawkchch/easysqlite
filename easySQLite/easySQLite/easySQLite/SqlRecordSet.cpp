#include "SqlRecordSet.h"


namespace sql
{

RecordSet::RecordSet(Database &db) :
  RecordSet(db.getHandle())
{
  _db = &db;
}

RecordSet::RecordSet(Database &db, const Field* definition) :
  RecordSet(db.getHandle(), definition)
{
  _db = &db;
}

RecordSet::RecordSet(Database &db, const FieldSet* fields) :
  RecordSet(db.getHandle(), fields)
{
  _db = &db;
}

RecordSet::RecordSet(sqlite3* db)
  : _fields(NULL)
{
  _db = NULL;
  _handle = db;
  _err_msg.clear();
  _result_query = SQLITE_ERROR;
  _records.clear();
}

RecordSet::RecordSet(sqlite3* db, const FieldSet* fields)
	: _fields(*fields)
{
  _db = NULL;
	_handle = db;
	_err_msg.clear();
	_result_query = SQLITE_ERROR;
	_records.clear();
}

RecordSet::RecordSet(sqlite3* db, const Field* definition)
	: _fields(definition)
{
  _db = NULL;
	_handle = db;
	_err_msg.clear();
	_result_query = SQLITE_ERROR;
	_records.clear();
}

RecordSet::~RecordSet(void)
{
	close();
}

string RecordSet::errMsg()
{
	return _err_msg;
}

void RecordSet::close()
{
	_err_msg.clear();
	_records.clear();
	_result_query = SQLITE_ERROR;
}

const FieldSet* RecordSet::fields() const
{
	return &_fields;
}

FieldSet* RecordSet::fields()
{
  return &_fields;
}

bool RecordSet::isResult()
{
	return (_result_query == SQLITE_OK);
}

int RecordSet::count()
{
	return _records.size();
}

int RecordSet::on_next_record(void* param, int column_count, char** values, char** columns)
{
	RecordSet* recordset = (RecordSet*)param;

	Record record(recordset->fields());
  
  if (column_count > recordset->fields()->count()) {
    record.initColumnCount(column_count);
  }
  
  int fieldIndex = 0;

  for (int index = 0; index < column_count; index++, fieldIndex++)
	{
		char* value = values[index];
    
    const Field *field = recordset->_fields.getByIndex(fieldIndex);
    while (field) {
      if (!field->isIgnored()) break;
      
      ++fieldIndex;
      field = recordset->_fields.getByIndex(fieldIndex);
    }
    
    if (!field) {
      char* column = columns[index];
      string fieldDefinitionMismatch = "RecordSet::query field definition mismatch column=" + string(column) + ", value=" + string(value);
      if (recordset->_db) {
        Database::Trace *tracer = recordset->_db->getTracer();
        if (tracer) {
          tracer->notifyDatabaseTrace(Database::Trace::Error, fieldDefinitionMismatch.c_str());
        }
      }

      THROW_EXCEPTION("RecordSet::query: " + fieldDefinitionMismatch)
    }

    record.initColumnValue(fieldIndex, value, field->getType());
  }

  if (recordset->_db) {
    Database::Trace *tracer = recordset->_db->getTracer();
    if (tracer) {
      tracer->notifyDatabaseTrace(Database::Trace::Informational, record.toString().c_str());
    }
  }

	recordset->_records.push_back(record);

  return DATASET_ITERATION_CONTINUE;
}

bool RecordSet::query(string sql)
{
	close();

	char* error = NULL;

	_result_query = sqlite3_exec(_handle, sql.c_str(), on_next_record, this, &error);

	if (isResult())
	{
		return true;
	}

	if (error)
	{
		_err_msg = error;
		sqlite3_free(error);
	}

  if (_db) {
    Database::Trace *tracer = _db->getTracer();
    if (tracer) {
      tracer->notifyDatabaseTrace(Database::Trace::Error, errMsg().c_str());
      tracer->notifyDatabaseTrace(Database::Trace::Warning, sql.c_str());
    }
  }

	THROW_EXCEPTION("RecordSet::query: " + errMsg())

	return false;
}

Record* RecordSet::getRecord(int record_index)
{
	if ((record_index >= 0) && (record_index < (int)_records.size()))
		return &_records.at(record_index);

	return NULL;
}

string RecordSet::toString()
{
	string s;

	for (int record_index = 0; record_index < count(); record_index++)
	{
		if (Record* record = getRecord(record_index))
		{
			s += intToStr(record_index + 1) + ". " + record->toString();
			s += "\r\n";
		}
	}

	return s;
}

Record* RecordSet::getTopRecord()
{
	if (isResult())
	{
		return getRecord(0);
	}
	return NULL;
}

Value* RecordSet::getTopRecordFirstValue()
{
	if (isResult())
	{
		if (Record* record = getRecord(0))
		{
			return record->getValue(0);
		}
	}
	return NULL;
}


//sql eof
};
