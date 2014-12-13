#include "SqlDatabase.h"
#include "SqlRecordset.h"
#include <time.h>


namespace sql
{

  Database::Database(Trace *tracer)
{
	_handle = NULL;
	_result_open = SQLITE_ERROR;
  _tracer = tracer;

	close();

#ifdef _WIN32
	_tzset();
#else //_WIN32
  tzset();
#endif //_WIN32
}

Database::~Database(void)
{
	close();
}

sqlite3* Database::getHandle() const
{
	return _handle;
}

Database::Trace *Database::getTracer() const
{
  return _tracer;
}

string Database::errMsg()
{
	return _err_msg;
}

void Database::close()
{
	if (_handle)
	{
    sqlite3_trace(_handle, NULL, NULL);
		sqlite3_close(_handle);
		_handle = NULL;
		_err_msg.clear();
		_result_open = SQLITE_ERROR;
	}
}

bool Database::isOpen()
{
	return (_result_open == SQLITE_OK);
}

bool Database::open(string filename)
{
	close();

	_result_open = sqlite3_open(filename.c_str(), &_handle);

	if (isOpen())
	{
    sqlite3_trace(_handle, trace, (void *)this);
		return true;
	} else {
		_err_msg = sqlite3_errmsg(_handle);
	}

	THROW_EXCEPTION("Database::open: " + errMsg())

	return false;
}

bool Database::transactionBegin()
{
	RecordSet rs(*this);

	if (rs.query("BEGIN TRANSACTION"))
		return true;

	return false;
}

bool Database::transactionCommit()
{
	RecordSet rs(*this);

	if (rs.query("COMMIT TRANSACTION"))
		return true;

	return false;
}

bool Database::transactionRollback()
{
	RecordSet rs(*this);

	if (rs.query("ROLLBACK TRANSACTION"))
		return true;

	return false;
}

void Database::trace(void *context, const char *message)
{
  Database *pThis = (Database *)context;
  if (!pThis) return;
  if (!pThis->_tracer) return;
  pThis->_tracer->notifyDatabaseTrace(Trace::Informational, message);
}

//sql eof
};
