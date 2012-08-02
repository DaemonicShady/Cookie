#ifndef __VAO__
#define __VAO__

class VAO
{
public:
	VAO  ();
	~VAO ();

	bool isOk () const;
	void bind ();
	void unbind ();

private:
	unsigned int   id_;
};

#endif