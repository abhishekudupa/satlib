#include <SatLibExceptions.hpp>
#include <ExpressionBase.hpp>

namespace SatLib {

    SatLibExceptionBase::SatLibExceptionBase(const string& ExceptionInfo) throw ()
        : ExceptionInfo(ExceptionInfo)
    {
        // Nothing here
    }
    
    SatLibExceptionBase::SatLibExceptionBase() throw ()
    {
        // Nothing here
    }

    SatLibExceptionBase::~SatLibExceptionBase() throw ()
    {
        // Nothing here
    }

    const char* SatLibExceptionBase::what() const throw ()
    {
        return ExceptionInfo.c_str();
    }

    const string& SatLibExceptionBase::GetExceptionInfo() const
    {
        return ExceptionInfo;
    }

    const string& SatLibExceptionBase::ToString() const
    {
        return ExceptionInfo;
    }

    InvalidExpressionException::InvalidExpressionException(const string& ExceptionInfo)
        : SatLibExceptionBase(ExceptionInfo)
    {
        // Nothing here
    }

    InvalidExpressionException::~InvalidExpressionException() throw ()
    {
        // Nothing here
    }

    ManagerMismatchException::ManagerMismatchException(const ExpressionBase* Exp)
    {
        ostringstream sstr;
        sstr << "Exception: Expression Manager of Exp: " << *Exp << " mismatched!";
        ExceptionInfo = sstr.str();
    }

    ManagerMismatchException::~ManagerMismatchException() throw ()
    {
        // Nothing here
    }

    SolverException::SolverException(const string& ExceptionInfo)
        : SatLibExceptionBase(ExceptionInfo)
    {
        // Nothing here
    }

    SolverException::~SolverException() throw ()
    {
        // Nothing here
    }

    PluginLoaderException::PluginLoaderException(const string& ExceptionInfo)
        : SatLibExceptionBase(ExceptionInfo)
    {
        // Nothing here
    }

    PluginLoaderException::~PluginLoaderException() throw ()
    {
        // Nothing here
    }

    InternalError::InternalError(const string& ExceptionInfo) 
        : SatLibExceptionBase(ExceptionInfo)
    {
        // Nothing here
    }

    InternalError::~InternalError() throw ()
    {
        // Nothing here
    }

    ostream& operator << (ostream& os, const SatLibExceptionBase& Ex)
    {
        os << Ex.ToString();
        return os;
    }

} /* End namespace */

