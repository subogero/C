#include <cstdio>
class Base {
	public:
	void hello() { printf("Base::hello\n"); }
	virtual void ciao() { printf("Base::ciao\n"); }
};

class Child: public Base {
	public:
	void hello() { printf("Child::hello\n"); }
	virtual void ciao() { printf("Child::ciao\n"); }
};

class Brat: public Base {
	public:
	void hello() { printf("Brat::hello\n"); }
};

int main(void)
{
	Base base;
	Child child;

	base.hello();
	base.ciao();

	printf("\n");
	child.hello();
	child.ciao();

	printf("\n");
	Base *foo = new Child();
	foo->hello();
	foo->ciao();

	printf("\n");
	Base *bar = new Brat();
	bar->hello();
	bar->ciao();
	return 0;
}
