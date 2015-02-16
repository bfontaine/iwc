# iwc(1)

[![Build Status](https://travis-ci.org/bfontaine/iwc.svg?branch=master)](https://travis-ci.org/bfontaine/iwc)

`iwc(1)` is an *interactive* `wc(1)`. It (mostly) works like the original `wc`
but prints the lines number as it counts them instead of waiting for the end to
print it.

![](https://github.com/bfontaine/iwc/raw/master/imgs/iwc.png)

## Install

### With Homebrew (OS X)

    brew tap bfontaine/utils
    brew install iwc

### From the source

    git clone https://github.com/bfontaine/iwc.git
    cd iwc
    [sudo] make install

## Usage

    $ iwc myfile.txt

See [the man page][man] to see all available options.

[man]: https://bfontaine.github.io/iwc

## License

[MIT][LICENSE].

[LICENSE]: https://github.com/bfontaine/iwc/blob/master/LICENSE
