#ifndef __APPLICATION__
#define __APPLICATION__

#include <string>

class Model;
class View;
class Controller;

using std::string;

class Application
{
public:
	Application ( string appName, HINSTANCE hInstance );
	~Application ();

	virtual bool isOk () const;
	virtual int  run  ();

	void setModel      ( Model* );
	void setView       ( View* );
	void setController ( Controller* );

protected:
	string      appName_;
	HINSTANCE   hInstance_;
	Model*      model_;
	View*       view_;
	Controller* controller_;
};

#endif