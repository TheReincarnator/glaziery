#ifndef __APPLICATION_H
#define __APPLICATION_H


/**
 * The one application object.
 */
extern FontCreator Application;


/**
 * The Application object represents the whole application.
 * You can store any data here that is more general than a single document.
 * @author Thomas Jacob
 */
class FontCreator : public TriExtApp
{
	private:

	// Declaration macros require their own protected section.
	protected:

		/**
		 * MFC dynamic object creation declaration.
		 */
		DECLARE_MESSAGE_MAP()

	public:

		/**
		 * Creates a new application.
		 */
		FontCreator();

		/**
		 * Destroys the application.
		 */
		~FontCreator();

		/**
		 * Initializes the current instance of the application.
		 * This method is called each time the application is concurrently started.
		 * @return 1 if successful, 0 otherwise.
		 */
		virtual int InitInstance();
};


#endif
