# iwc(1) -- Interactive lines count

## SYNOPSIS

`iwc` [`-chlvw`] [<file> ...]

## DESCRIPTION

The `iwc` utility displays the number of lines, words and bytes in the standard
input. The number is printed while `iwc` counts all lines, instead of just
displaying the total number at the end.

## OPTIONS

Not passing any option is equivalent to passing `-clw`.

  * `-c`:
    Count the number of bytes in each input file.

  * `-h`:
    Print the help and exit

  * `-l`:
    Count the number of lines in each input file.

  * `-m`:
    Same as `-c` for now.

  * `-v`:
    Print the version and exit

  * `-w`:
    Count the number of words in each input file.

## AUTHOR

Written by Baptiste Fontaine <<b@ptistefontaine.fr>\>.

## BUGS

Bugs should be reported at <https://github.com/bfontaine/iwc/issues>.

## SEE ALSO

The project homepage is at <https://github.com/bfontaine/iwc>.
