# DSA2020 Email Searcher
Data Structure and Algorithm, Spring 2020

## Introduction
This is the final project of DSA lecture in NTU CSIE:

>The main theme of the project is a email searcher. The email searcher helps retrieving precious emails from the mail pool, which contains mails that came in a streaming manner.
The searcher should effectively use the computational and storage resources of your computer. So the data structure (and the associated algorithm) used for representing the mail pool can be crucial.
>--<cite>[Project][1]</cite>

[1]: https://www.csie.ntu.edu.tw/~htlin/course/dsa20spring/project/

For detailed input and output format, please refer to `./doc/project.pdf` and `./doc/project_spec.pdf`

### Operations
**Words in this section is from `./doc/project.pdf` and `./doc/project_spec.pdf`**
- `add <file>`: add a mail into database, where `<file>` is the absolute path of the file that stores a mail.

- `remove <id>`: remove mail from database, where `<id>` is the unique positive Message-ID of a mail.
  
- `longest`: return the Message-ID of mail whose content contains the largest number of alpha-numeric characters.

- `query [-f"<from>"] [-t"<to>"] [-d<date>~<date>] <expression>`: return all the Message-ID of mails that fits the query description, where
```
<from> := <string>
<to> := <string>
<date> := {YYYYMMDDhhmm} or {blank}
<expression> := <keyword> or (<expression>) or !<expression>
or <expression>|<expression> or <expression>&<expression>
<keyword> := <string>
<string> := {any alpha-numeric string without whitespace or punctuation}
```

### Survey
>Study at least THREE data structures for dealing with the email searcher. Then, make a comparison of those data structures according to some different perspectives, such as average speed, worst speed, space, implementation, popularity, etc.. Based on the results of your comparison, you are asked to recommend the best one for the email searcher, and provide the "cons and pros" of the choice.
>--<cite>[Project][1]</cite>

[1]: https://www.csie.ntu.edu.tw/~htlin/course/dsa20spring/project/

We conducted the survey of:
- Dynamic Array: `std::vector`

- Red Black Tree: `std::map` and `std::set`

- Hash Table: `__gnu_pbds::cc_hash_table`

For detailed survey report, please refer to  `./doc/report.pdf`

## How to Run
### Executables of the Searcher
Run `make` and generate two executable files:
- `run` (based on hash table and rb-tree)

- `set` (based on only rb-tree)

### Extract the Archive of Email Data
Run
```shell=
tar -xf ./test/mails.tar.gz
```
and get `test_data` folder which contains 10,000 mails.

### Generate Tested Input 
Run
```shell=
g++ gen_test_data.cpp -o generator
```
to get the executable file.

Then run
```shell=
./generator > input
```
to get the input file.

The input file could be executed with the searcher:
```shell=
./run < input > output1
./set < input > output2
```

## Contributions
I implemented two kinds of Email Searchers, one based on only rb-tree, the other based on rb-tree and hash table. Discussed only rb-tree in the report.

Ji-Yun Tu implemented an Email Searcher based on rb-tree and hash table and accelerated input and output. Discussed hash table in the report.

朱軒葶 implemented an Email Searcher based on dynamic array and discussed it in the report.

## Resources
https://www.csie.ntu.edu.tw/~htlin/course/dsa20spring/project/
