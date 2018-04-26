/*
 * FILE: singleton.hpp
 * DESC: singleton pattern template
 * AUTHOR: okseok.kim@gmail.com
 * CDATE: 2018-04-22
 * UDATE: 2018-04-22
 * LICENSE: Bryllite Ltd. All rights reserved.
 * NOTES: 명시적으로 할당/제거를 관리해야 하는 전역 클래스 사용시에만 사용하도록 한다.
 * USAGE:

	// 싱글턴 인스턴스 클래스 선언
	class CMyInstance : public Bryllite::Singleton< CMyInstance > 
	{
		friend class Bryllite::Singleton< CMyInstance > ;
	protected:
		CMyInstance() {} ;
		virtual ~CMyInstance() {} ;

	public:
		void something_todo( void ) { std::cout << "Hello, Bryllite!" << std::endl ; } ;
	};

	// 사용
	int main( void ) {
		CMyInstance.getInstance()->something_todo() ;
		CMyInstance.releaseInstance() ;	// 명시적으로 제거하지 않아도 종료시에 제거된다.
		return 0 ;
	};

 */

#pragma once

#include <stdlib.h>

// namespace: bryllite
namespace bryllite {

// Singleton template
template< typename T >
class Singleton
{
public:
	static T* getInstance( void ) ;
	static void releaseInstance( void ) ;
	static bool isAllocated( void ) ;

protected:
	static T* m_pInstance ;
};

// initialize the instance to null
template< typename T > T* Singleton<T>::m_pInstance = NULL ;

// 명시적 인스턴스 생성
template< typename T > 
T* Singleton<T>::getInstance( void )
{
	if ( m_pInstance == NULL )
	{
		m_pInstance = new T();

		// 명시적 제거를 깜빡하더라도, 프로그램 종료시에 자동으로 인스턴스 제거
		// @WARNING: 인스턴스 제거 순서가 중요한 경우, 오류가 발생할 수 있으므로 반드시 명시적 인스턴스 제거
		atexit( releaseInstance ) ;
	}

	return m_pInstance ;
};

// 명시적 인스턴스 제거
template< typename T >
void Singleton<T>::releaseInstance( void )
{
	if ( m_pInstance != NULL )
		delete m_pInstance ;

	m_pInstance = NULL ;
};

// test for instance allocated or not
template< typename T >
bool Singleton<T>::isAllocated( void )
{
	return m_pInstance ? true : false ;
};

};	// namespace bryllite


