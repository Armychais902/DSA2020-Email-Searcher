# DSA2020 Email Searcher
Data Structure and Algorithm, Spring 2020

## Introduction
This is the final project of DSA lecture in NTU CSIE:

The main theme of the project is a email searcher. The email searcher helps retrieving precious emails from the mail pool, which contains mails that came in a streaming manner.
The searcher should effectively use the computational and storage resources of your computer. So the data structure (and the associated algorithm) used for representing the mail pool can be crucial.

### Operations
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
### 

### Survey
Study at least THREE data structures for dealing with the email searcher. Then, make a comparison of those data structures according to some different perspectives, such as average
speed, worst speed, space, implementation, popularity, etc.. Based on the results of your comparison, you
are asked to recommend the best one for the email searcher, and provide the \cons and pros" of the
choice.

## 

## Contributions
