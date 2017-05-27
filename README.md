# Tcl binding to BearSSL

This project is a [Tcl](https://www.tcl.tk/) binding to the excellent [BearSSL](https://bearssl.org)
library being developed by Thomas Pornin.

*WARNING: THIS SHOULD NOT BE USED IN PRODUCTION!*

As per the documentation on that library:

> Current version is 0.4. It is considered alpha-quality software, which means that it runs but it probably has bugs,
> some of which being certainly exploitable vulnerabilities. A lot more testing and documentation is needed before
> BearSSL can be deemed usable in production.

This binding is being provided on the assumption that it is still useful for testing and
prototyping purposes. One day BearSSL will be production-ready, and then this extension will be too.
The design approach, code quality and authorship of BearSSL make me hopeful that we will not need to
wait too long.

## License

Bear is licensed under MIT license, as is this binding. See [LICENSE.md](LICENSE.md) for details of the licensing
terms for this source code and check the BearSSL sources for the license terms associated with your copy.

## Building

1. Build BearSSL
2. Edit `Makefile` to set appropriate flags for your system (sorry).
3. `make clean all`
4. Copy `libtclbearssl[info sharedlibextension]` and `pkgIndex.tcl` somewhere on your `auto_path`.
