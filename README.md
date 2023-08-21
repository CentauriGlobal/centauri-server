# centauri

## Indexes:

* midas_billing_scripts: postman collections and environment variables used to interact with Midas system. To use this script, you need to install 'pmlib_code'.
* sdk: golang library used to perform signing and verifying.
* server: do not need to focus on.
* sample: samples used to show you how to request.

## How to install pmlib_code

Add a new request:

method: GET
url: https://joolfe.github.io/postman-util-lib/dist/bundle.js

and add the following code to tests part:

```js
pm.test("Status code should be 200", function () {
    pm.response.to.have.status(200)
    pm.globals.set("pmlib_code", responseBody)
});
```

then send a request to install it in your postman.

