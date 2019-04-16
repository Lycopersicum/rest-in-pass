# REST API

## Authorization interface
This interface is dedicated for authorizing various requests scope.
If you restrict access to any requests URLs group, for example, keeper,
then everyone doing requests to `keeper` will have to authorize their requests.
### Perform request with authorization
* **URL**
 any URL with restricted access
* **Success Response:** whatever code you would get from regular request <br />
* **Error Response:**
  - `403 FORBIDDEN` - provided authorization key is invalid.
* **Sample Call:**
  ```shell
  curl -X GET http://localhost:80/keeper/creative-name' -H "Authorization: Basic "
  ```
  
### Perform request with authorization
What happens when proper authorization details aren't provided?
* **URL**
 any URL with restricted access
* **Error Response:**
  - `401 UNAUTHORIZED` - provided authorization key is missing.
  Also there is `WWW-Authenticate` header, which tells, which realm secret is required.
* **Sample Call:**
  ```shell
  curl -X GET http://localhost:80/keeper/creative-name'
  ```

## Keeper interface
This interface is dedicated for Create, Read, Update and Delete password manipulations.
### Create new password
* **URL**
 `/keeper`
* **Method:**
  `POST`
* **Data Parameters**
  *JSON object* with keys:
    - `name` _(string)_: pick a name for your secret password
    - `secret` _(string)_: your secret password
* **Success Response:** 
  - `201 CREATED` - Successfully created new password instance.
* **Error Response:**
  - `400 BAD REQUEST` - something is wrong with provided data, maybe description is missing something?
  - `409 CONFLICT` - password with this name is already registered.
  - `415 UNSUPPORTED_MEDIA_TYPE` - either provided data isn't JSON or `Content-Type` header is invalid/missing.
* **Sample Call:**
  ```shell
  curl -X POST http://localhost:80/keeper -H "Content-Type: application/json" --data '{"name":"creative-name", "secret": "some-string"}'
  ```
  
### Read password
* **URL**
 `/keeper/creative-name`
* **Method:**
  `GET`
* **Success Response:**
  - `200 OK` - reading password secret was successful. 
    **Content:** `"some-string"`
* **Error Response:**
  - `400 BAD REQUEST` - something is wrong with provided data, maybe payload is corrupt?
  - `404 NOT FOUND` - password with this name isn't registered.
  - `415 UNSUPPORTED_MEDIA_TYPE` - either provided data isn't plaintext or `Content-Type` header is invalid/missing.
* **Sample Call:**
  ```shell
  curl -X GET http://localhost:80/keeper/creative-name'
  ```
  
### Update password
* **URL**
 `/keeper/creative-name`
* **Method:**
  `PUT`
* **Data Parameters**
  - `secret` _(string)_: your secret password
* **Success Response:**
  - `204 NO CONTENT` - successfully changed password secret. 
* **Error Response:**
  - `400 BAD REQUEST` - something is wrong with provided data, maybe payload is corrupt?
  - `404 NOT FOUND` - password with this name isn't registered.
  - `415 UNSUPPORTED_MEDIA_TYPE` - either provided data isn't plaintext or `Content-Type` header is invalid/missing.
* **Sample Call:**
  ```shell
  curl -X PUT http://localhost:80/keeper/creative-name -H "Content-Type: text/plain" --data 'some-other-string'
  ```
  
### Delete password
* **URL**
 `/keeper/creative-name`
* **Method:**
  `DELETE`
* **Success Response:**
  - `204 NO CONTENT` - successfully deleted password instance. 
* **Error Response:**
  - `404 NOT FOUND` - password with this name isn't registered.
* **Sample Call:**
  ```shell
  curl -X DELETE http://localhost:80/keeper/creative-name'
  ```
  
