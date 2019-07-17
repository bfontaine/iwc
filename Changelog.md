# iwc Changelog

## Unreleased

* Optimize `iwc -c` by getting the file size when we can

## v0.1.3 (2016-09-16)

* Files are now properly closed.

## v0.1.2 (2016-05-09)

* Words count fixed, it now matches `wc`’s.

## v0.1.1 (2015-02-17)

* The read buffer is now 16k instead of 1k
* The newlines are now searched with `memchr` instead of manually looping on
  the buffer.
* Improved words count
* Improved formatting

## v0.1.0 (2015-02-16)

* Words & bytes counts
* Code split into two files (`cli.c`, `iwc.c`)
* Command-line options added
* Support for multiple input files (not only the standard input)
* Proper error handling

## v0.0.1 (2015-02-15)

Initial release, as a proof-of-concept
