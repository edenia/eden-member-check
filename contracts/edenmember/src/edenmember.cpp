#include "../include/edenmember.hpp"

namespace edenia {
  void edenmember::checkmember( name account ) {
    check( is_account( account ), "Account does not exist." );
    check( is_eden( account ), "Given account is not an Eden Member" );
  }

  void edenmember::isedenmember( name account_to_notify,
                                 name account_to_review ) {
    require_auth( account_to_notify );

    SEND_INLINE_ACTION( *this,
                        resultiseden,
                        { get_self(), "active"_n },
                        { account_to_notify,
                          account_to_review,
                          is_eden( account_to_review ) } );
  }

  void edenmember::resultiseden( name account_to_notify,
                                 name account_to_review,
                                 bool is_eden ) {
    require_auth( get_self() );
    require_recipient( account_to_notify );
  }
} // namespace edenia

EOSIO_ACTION_DISPATCHER( edenia::actions )

EOSIO_ABIGEN( actions( edenia::actions ),
              ricardian_clause( "datastorage", edenia::datastorage_clause ),
              ricardian_clause( "datausage", edenia::datausage_clause ),
              ricardian_clause( "dataownership", edenia::dataownership_clause ),
              ricardian_clause( "datadistribution",
                                edenia::datadistribution_clause ),
              ricardian_clause( "datafuture", edenia::datafuture_clause ) )