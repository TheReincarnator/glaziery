#ifndef __APPLICATION_H
#define __APPLICATION_H


#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	class Application : public CObject
#else
	class Application
#endif
{
	private:

		static Application * singleton;

		Application();
		~Application();

	public:

		static void createInstance();
		static void destroyInstance();
		static Application * getInstance();
};


#endif
