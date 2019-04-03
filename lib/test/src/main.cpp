/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <ellcpp/cutting_plane.hpp>
#include <ellcpp/ell.hpp>
#include <ellcpp/oracles/profit_oracle.hpp>
#include <xtensor/xarray.hpp>

// using namespace fun;

TEST_CASE("Profit Test 1", "[profit]") {
    using Vec = xt::xarray<double>;

    auto p = 20., A = 40., k = 30.5;
    auto a = Vec{0.1, 0.4};
    auto v = Vec{10., 35.};

    {
        auto E = ell(100., Vec{0., 0.});
        auto P = profit_oracle(p, A, k, a, v);
        auto [_, fb, niter, feasible, status] = cutting_plane_dc(P, E, 0.);
        CHECK(niter == 37);
    }

    {
        auto ui = 1., e3 = 1.;
        auto e = Vec{0.003, 0.007};

        auto E = ell(100., Vec{0., 0.});
        auto P = profit_rb_oracle(p, A, k, a, v, ui, e, e3);
        auto [_, fb, niter, feasible, status] = cutting_plane_dc(P, E, 0.);
        CHECK(niter == 42);
    }

    {
        auto E = ell(100., Vec{2, 0.});
        auto P = profit_q_oracle(p, A, k, a, v);
        auto [_, fb, niter, feasible, status] = cutting_plane_q(P, E, 0.);
        CHECK(niter == 28);
    }
}

// TEST_CASE( "Projective Point", "[proj_plane]" ) {
//     CHECK( l.incident({l, m}) );
// }

// int main(int argc, char* argv[]) {
//   //using namespace ModernCppCI;

//   auto result = Catch::Session().run(argc, argv);

//   return (result < 0xff ? result : 0xff);
// }
