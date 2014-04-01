// Defines a templated Singleton class
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

// You must place this macro inside the class definition of your derived Singleton
#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

namespace ITP485
{

template <class T>
class Singleton
{
private:
	static T* _instance;
protected:
	Singleton() {}
public:
	static T& get()
	{
		if (_instance)
		{
			return *_instance;
		}
		else
		{
			_instance = new T();
			return *_instance;
		}
	}
};

template <class T> T* Singleton<T>::_instance = 0;

} // namespace ITP485

#endif // _SINGLETON_H_
