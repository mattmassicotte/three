####Three

Three is intended for systems and low-level programming that would typically be done with C.  When compared to C, Three's goals are:

- Closures
- Namespacing
- Reduced macro usage
- Standardization of conditional compilation
- Language-level constructs for atomic and transactional memory operations
- Explicit control over mutability and nullable pointers
- Standardized and simplified attributes
- Vector instructions
- Linkage and ABI controls
- API documentation notation
- Library+header packaging
- 95% native compatibility with existing C headers

####Warning

Three is still in active development. You will encounter compiler bugs, including codegen problems. Please be careful using Three, and do report problems you find!

####Why does Three exist?

C is an awesome and unparalleled language.  But, it can get a little messy at times, particularly when the preprocessor gets involved.  Three is an attempt to do nearly everything C can do, but with a simpler, standard syntax.

####Building and Installing

Three uses a custom build system written with [Rake](http://rake.rubyforge.org/).  That means you need ruby, rake, and the ruby gem rake-compile.  In addition to those, you also need a C++ compiler that supports C++11.  The build system will automatically download an install other dependencies.  These include LLVM, so be ready for a rather lengthy build the first time through.

To build and install:

    $ git clone git@github.com:mattmassicotte/three.git
    $ cd three
	$ gem install rake-compile
    $ rake install

####Contributing

You are, of course, very welcome to contribute.  Pull requests are great, but so are issues.  Everything helps!

- @mattmassicotte
- @kb

####License

MIT, see license.txt
