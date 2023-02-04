#include <fmt/printf.h>
#include <thread>
#include <chrono>
#include <tuple>

constexpr double investment        = 100.0;
constexpr double yearly_return      = 0.10;
constexpr int investment_period    = 10;

auto calc_profit() -> std::tuple<double, double>
{
    static double cur_pfolio = 0;
    auto last_year_pfolio = cur_pfolio;
    cur_pfolio += (cur_pfolio)*yearly_return;
    auto tmp = cur_pfolio;
    cur_pfolio += investment * 12;

    return {cur_pfolio, (tmp - last_year_pfolio)};
};


auto main(int args, char* argv[]) -> int{
    size_t yrs = 0;
    for (;;) {
        yrs++;
        auto profit = calc_profit();
        fmt::print("cur_pfolio={}\tprofit={}\t\t({} yr(s))\n", std::get<0>(profit), std::get<1>(profit), yrs);
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2000ms);
        }
        if (yrs >= investment_period)
            break;
    }

    return 0;
}
