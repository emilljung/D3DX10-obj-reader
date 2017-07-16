#ifndef _WORKINGOBJECT_H__
#define _WORKINGOBJECT_H__
#include "D3DObject.h"
#include "BTH.h"

// Written by Tommy Petersson

class WorkingObject : public D3DObject
{
	public:
		WorkingObject ( HINSTANCE hInstance );
		virtual ~WorkingObject ();

		bool initiateAndRun ();

		virtual void update ( float dt );
		virtual void render ();

	private:
		BTH bth;
};

#endif // _WORKINGOBJECT_H__