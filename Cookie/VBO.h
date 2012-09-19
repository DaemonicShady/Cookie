#ifndef __VBO__
#define __VBO__

typedef unsigned int GLenum;

class VBO
{
public:
	VBO  ();
	~VBO ();

	unsigned getId () const;
	void  bind ( GLenum target );
	void  unbind ();

	void  setData    ( unsigned size, const void* data, GLenum usage );
	void  setSubData ( unsigned offset, unsigned size, const void* data );
	void  getSubData ( unsigned offset, unsigned size, void* data );

	void* map   ( GLenum access );
	bool  unmap ();
	void  clear ();

private:
	unsigned id_;
	GLenum   target_;
	bool     ok_;
};

#endif