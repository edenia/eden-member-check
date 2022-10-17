#include "../include/edenmember.hpp"

namespace edenia {
    void edenmember::checkmember(name account) {
        check( is_account(account), "Account does not exist.");
        check( is_eden(account), "Given account is not an Eden Member" );
    }
} // namespace edenia


EOSIO_ACTION_DISPATCHER(edenia::actions)

EOSIO_ABIGEN(
    actions(edenia::actions),
    ricardian_clause("datastorage", edenia::datastorage_clause),
    ricardian_clause("datausage", edenia::datausage_clause),
    ricardian_clause("dataownership", edenia::dataownership_clause),
    ricardian_clause("datadistribution", edenia::datadistribution_clause),
    ricardian_clause("datafuture", edenia::datafuture_clause)
)