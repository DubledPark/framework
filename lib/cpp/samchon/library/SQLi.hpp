#pragma once
#include <samchon/API.hpp>

#include <memory>
#include <mutex>
#include <string>

namespace samchon
{
namespace library
{
	class SQLStatement;

	/**
	 * @brief A SQL interface; DBMS connector
	 *
	 * @details
	 * <p> SQLi is an abstract class managing connection (session) with a specific database and manages
	 * SQL statements are executed and results are returned within the context of a connection. </p>
	 *
	 * <p> The reason why SQLStatement(s) are managed by SQLi is because of a principle of DBMS, 
	 * DBMS system doesn't allow simultaneous query from a session (connection). Only a query (process0) 
	 * is allowed at a time. If you try to simultaneous query from a SQLi, ODBC throws error. To avoid 
	 * the error, an execute of query from SQLStatement will lock a mutex of SQLi to ensure 
	 * exclusiveness. </p>
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_data.png)
	 *
	 * @note
	 * <p> To ensure the exclusiveness, you've to make SQLStatement from SQLi. Do not make SQLStatement 
	 * by yourself. call SQLi::createSQLStatement() instead. Even you make a derived class from
	 * SQLStatement, don't make its constructor to have public accessor. </p>
	 *
	 * <p> Becuase execution of a sql statement causes lock on mutex in SQLi, you've to destruct the
	 * SQLStatement or call SQLStatement::free() method(). If you don't, the mutex will not be unlocked,
	 * thus you can't do anything by the SQLi. </p>
	 * 
	 * @handbook [Library - SQL Driver](https://github.com/samchon/framework/wiki/CPP-Library-SQL_Driver)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API SQLi
	{
		friend class SQLStatement;

	protected:
		/**
		 * @brief Driver; name of DBMS used for connection
		 *
		 * @details
		 *  \li MySQL: \"{MySQL Server}\"
		 *	\li MS-SQL: \"{SQL Server}\"
		 *	\li PL-SQL: \"{Oracle}\"
		 */
		std::string driver;

		/**
		 * @brief Port number of DBMS
		 *
		 * @details
		 *	\li MySQL: 3306
		 *	\li MS-SQL: 1433
		 *	\li PL-SQL: 1521
		 */
		int port;

	private:
		/**
		 * @brief Handler of DB-connector
		 */
		void *hdbc;

		/**
		 * @brief SQLStatement's pointer linked with the SQLi
		 */
		SQLStatement *stmt;

		/**
		 * @brief Mutex ensuring concurrency with SQLStatement
		 */
		std::mutex stmtMutex;

		/**
		 * @brief Had connected to DBMS.
		 *
		 * @details 
		 * #connected is a flag variable for connection to DBMS. However, it doesn't mean the real-time
		 * status. #connected only stores whether a connection was succeeded or not.
		 */
		bool connected;

	public:
		/**
		 * @brief Construct from driver name and port
		 *
		 * @param driver Driver name of DBMS
		 * @param port Port number of DBMS
		 */
		SQLi(const std::string &driver, int port);

		virtual ~SQLi();

		/**
		 * @brief Connect to the DBMS\n
		 *
		 * @param ip An IP address of the DBMS
		 * @param db A name of database to access
		 * @param id Your own ID authorified from DBMS
		 * @param pwd A password for the ID
		 *
		 * @throw exception Error message from DBMS
		 */
		virtual void connect
			(
				const std::string &ip, const std::string &db,
				const std::string &id, const std::string &pwd
				);

		/**
		 * @brief Disconnect from DBMS
		 */
		virtual void disconnect();

		auto isConnected() const -> bool;

		/**
		 * @brief Factory method for creating SQLStatement
		 *
		 * @details
		 * Recommend to create SQLStatement by this method.
		 * Direct creation is not recommended as the reason of domain problem of each DBMS
		 *
		 * @return A SQLStatement matched for the domain SQLi
		 */
		virtual auto createStatement() -> std::shared_ptr<SQLStatement>;

	protected:
		/**
		 * @brief Get error message. 
		 * 
		 * @details Gets error message for throwing exception.
		 */
		virtual auto getErrorMessage(short type) const -> std::string;
	};
};
};