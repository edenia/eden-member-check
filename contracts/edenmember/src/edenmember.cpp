#include "../include/edenmember.hpp"

namespace edenia {
    void edenmember::addmember(name user) {
        require_auth(user);
        check( is_eden(user), "Given user is not an Eden Member" );

        member_table _member(_self, _self.value);
        auto member_itr = _member.find(user.value);
        check( member_itr == _member.end(), "Member already exist" );
        _member.emplace(user, [&](auto& row) {
            row.user = user;
        });
    }
} // namespace edenia


EOSIO_ACTION_DISPATCHER(edenia::actions)

EOSIO_ABIGEN(
    actions(edenia::actions),
    table("member"_n, edenia::member),
    ricardian_clause("datastorage", edenia::datastorage_clause),
    ricardian_clause("datausage", edenia::datausage_clause),
    ricardian_clause("dataownership", edenia::dataownership_clause),
    ricardian_clause("datadistribution", edenia::datadistribution_clause),
    ricardian_clause("datafuture", edenia::datafuture_clause)
)