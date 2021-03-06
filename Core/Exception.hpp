/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <cinttypes>
#include <exception>

#include <Core/logger/Logger.hpp>

namespace con {
/*
===============================================================================
Created by: Condzi
	Base class that is used for exceptions and assertions.

===============================================================================
*/
class BasicException
{
public:
	BasicException( std::string wFile, std::string wFunction, uint32_t wLine, std::string wCondition, std::string wMessage ) :
		file( std::move( wFile ) ), function( std::move( wFunction ) ), condition( std::move( wCondition ) ), message( std::move( wMessage ) ),
		line( std::move( wLine ) )
	{}
	virtual ~BasicException() = default;
	BasicException operator=( const BasicException& ) = delete;

	const std::string File() const
	{
		return this->file;
	}
	const std::string Function() const
	{
		return this->function;
	}
	const uint32_t Line() const
	{
		return this->line;
	}
	const std::string Condition() const
	{
		return this->condition;
	}
	const std::string Message() const
	{
		return this->message;
	}

private:
	const uint32_t line;
	const std::string file, function, condition, message;
};

class NotImplemented :
	public BasicException
{
public:
	NotImplemented( std::string wFile, std::string wFunction, uint32_t wLine, std::string wMessage ) :
		BasicException( wFile, wFunction, wLine, "no condition", wMessage )
	{}
};

inline void showBasicExceptionData( const BasicException& baseException )
{
	LOG( "File: " << baseException.File(), ERROR, BOTH );
	LOG( "Function: " << baseException.Function(), ERROR, BOTH );
	LOG( "Line: " << baseException.Line(), ERROR, BOTH );
	LOG( "Condition: " << baseException.Condition(), ERROR, BOTH );
	LOG( "Message: " << baseException.Message(), ERROR, BOTH );
}
}