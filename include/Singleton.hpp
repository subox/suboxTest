#ifndef __SUBOXTESTSINGLETON_HPP__
#define	__SUBOXTESTSINGLETON_HPP__

namespace suboxTest{

template < class T >
class Singleton{
	Singleton( Singleton const& );
	Singleton& operator=( Singleton const& );

protected:
	Singleton() {}
	virtual ~Singleton(){}

public:
	static T& getInstance(){
		static T instance;
		return instance;
	}
};
}

#endif //__SUBOXTESTSINGLETON_HPP__

