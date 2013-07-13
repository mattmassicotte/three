####Three

Three is intended for systems and low-level programming that would typically be done with C.  When compared to C, Three's goals are:

- Add Closures
- Add Namespacing
- Reduce macro usage
- Standardize method of conditional compilation
- Provide language-level constructs for atomic and transactional memory operations
- Standardize and simplify attributes
- Support vector instructions
- Standardize linkage and ABI controls
- Standardize API documentation notation

####Why does Three exist?

C is an awesome and unparalleled language.  But, it can get a little messy at times, particularly when the preprocessor gets involved.  Three is an attempt to do nearly everything C can do, but with a simpler, standard syntax.

####Building and Installing

Three uses a custom build system written with [Rake](http://rake.rubyforge.org/).  In addition to rake, you also need a version of clang installed that supports C++11.

To build and install:

    $ git clone git@github.com:mattmassicotte/three.git
    $ cd three
    $ rake install

####Contributing

You are, of course, very welcome to contribute.  Pull requests are great, but so are issues.  Everything helps!

- @mattmassicotte
- @kb

####License

MIT, see license.txt
