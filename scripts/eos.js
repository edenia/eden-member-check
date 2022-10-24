const EosApi = require("eosjs-api");

const { endpoint } = require("./config");

const eosApi = EosApi({
  httpEndpoint: endpoint,
  verbose: false,
  fetchConfiguration: {},
});

const getTableRows = (options) =>
  eosApi.getTableRows({ json: true, ...options });

module.exports = { getTableRows };
