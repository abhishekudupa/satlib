#if !defined __SATLIB_EXCEPTIONS_HPP
#define __SATLIB_EXCEPTIONS_HPP

#include "SatLibForwardDecls.hpp"

namespace SatLib {

    class SatLibExceptionBase : public exception
    {
    protected:
        string ExceptionInfo;

    public:
        SatLibExceptionBase() throw ();
        SatLibExceptionBase(const string& ExceptionInfo) throw ();
        virtual ~SatLibExceptionBase() throw ();
        virtual const char* what() const throw ();
        const string& GetExceptionInfo() const;
        virtual const string& ToString() const;
    };

    class InvalidExpressionException : public SatLibExceptionBase
    {
    public:
        InvalidExpressionException(const string& ExceptionInfo);
        virtual ~InvalidExpressionException() throw ();
        
    };

    class ManagerMismatchException : public SatLibExceptionBase
    {
    public:
        ManagerMismatchException(const ExpressionBase* Exp);
        virtual ~ManagerMismatchException() throw ();
    };

    class SolverException : public SatLibExceptionBase
    {
    public:
        SolverException(const string& ExceptionInfo);
        virtual ~SolverException() throw ();
    };

    class PluginLoaderException : public SatLibExceptionBase
    {
    public:
        PluginLoaderException(const string& ExceptionInfo);
        virtual ~PluginLoaderException() throw ();
    };

    class InternalError : public SatLibExceptionBase
    {
    public:
        InternalError(const string& ExceptionInfo);
        virtual ~InternalError() throw ();
    };

    ostream& operator << (const ostream& os, const SatLibExceptionBase& Ex);

} /* End namespace */

#endif /* __SATLIB_EXCEPTIONS_HPP */

