# rest-in-pass

[![Build Status](https://travis-ci.com/Lycopersicum/rest-in-pass.svg?branch=master)](https://travis-ci.com/Lycopersicum/rest-in-pass) [![codecov.io](http://codecov.io/github/Lycopersicum/rest-in-pass/coverage.svg?branch=master)](http://codecov.io/github/Lycopersicum/rest-in-pass?branch=master)

## Introduction
REST In Pass - Password storage with REST API.
Detailed [REST API documentation](./doc/REST_API.md).

## Building
Punica follows [scripts to rule them all](https://github.com/github/scripts-to-rule-them-all) guidelines, therefore getting dependencies,
building and testing is implemented by executing scripts, however if you want,
you can read [manual project build instructions](./doc/MANUAL_BUILD.md).

1. Download [punica/punica](https://github.com/punica/punica):
```
$ git clone --recursive https://github.com/Lycopersicum/rest-in-pass.git
$ cd rest-in-pass
```
_Note: If you already cloned Rest in Pass without initializing submodules, you can do so by executing:_
```
$ git submodule update --init --recursive
```

2. Set-up Rest In Pass:
```
$ script/setup
```
_Note: If script succeeds, you should have binary file called `rest-in-pass` in your `rest-in-pass/build/` directory._

## Usage
You can get some details about `rest-in-pass` by using `--help` or `-?` argument:
```
$ ./build/rest-in-pass --help
Usage: rest-in-pass [OPTION...]
REST In Pass - Password storage with REST API

  -c, --config=FILE          Specify configuration file
  -?, --help                 Give this help list
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
```

You can get some details about `punica` usage by using `--usage` argument:
```
$ ./build/punica --usage
Usage: rest-in-pass [-?] [-c FILE] [--config=FILE] [--help] [--usage]
```

### Arguments list:
- `-c FILE` and `--config FILE` is used to load configuration file.

     Example of configuration file is in configuration section (below)

### Configuration file:
_Please note that configuration file is **OPTIONAL**! That means, that server will work properly without configuration file, however it wont be secure (no encryption nor authentication), therefore it is highly **RECOMMENDED** to configure server properly._

Example of configuration file:
```
{
  "http": {
    "port": 4280,
    "authorization": [
      {
        "realm": "keeper",
        "secret": "dmVyeS1iYXNpYw=="
      }
    ]
  },
  "keeper": {
    "passwords_file": "passwords.json"
  }
}
```

- **`http` settings section _(object)_:**
  - `port` _(integer)_ - HTTP port to create socket on (is mentioned in arguments list). _**Optional**, default value is 80._
  - **`authorization` settings subsection _(list of objects)_:**
    - `realm` _(string)_ - Authorization scope name, also known as realm. _If you want to configure authorization, this option is **mandatory**._
    - `secret` _(base64 string)_ - Authorization scope secret. _If you want to configure authorization, this option is **mandatory**._

- **`keeper` settings section _(object)_:**
  - `passwords file` _(string)_ - File, where passwords would be backuped. _**Optional**, default value is NULL._

