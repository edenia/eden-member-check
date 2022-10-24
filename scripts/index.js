const { getTableRows } = require("./eos");

const isEden = async (account) => {
  const { rows } = await getTableRows({
    code: "genesis.eden",
    scope: 0,
    table: "member",
    lower_bound: account,
    upper_bound: account,
    limit: 1,
  });

  console.log({ rows: !!rows.length });

  return !!rows.length;
};

isEden("account");
