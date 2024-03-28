#include <chrono>
#include <fmt/color.h>
#include <fmt/printf.h>
#include <thread>
#include <tuple>

constexpr double investment = 500.0;
constexpr double yearly_return = 0.05;
std::vector<double> historical_returns = {
    -0.11, 0.01,  0.10,  0.15,  -0.17, -0.29,  0.31,    0.19, -0.11, 0.01,
    0.12,  0.25,  -0.09, 0.14,  0.17,  0.0140, 0.26,    0.14, 0.02,  0.12,
    0.27,  -0.06, 0.26,  0.04,  0.07,  -0.01,  0.34,    0.20, 0.31,  0.26,
    0.19,  -0.10, -0.13, -0.23, 0.26,  0.09,   0.03,    0.13, 0.035, -0.4,
    0.23,  0.12,  0,     0.13,  0.29,  0.11,   -0.0073, 0.09, 0.19,  -0.06,
    0.28,  0.16,  0.26,  -0.19, 0.24,  0.10};
constexpr int investment_period = 20;

static double cur_pfolio = 5000;
static double out_of_pocket_money = 5000;
static int year = -1;

auto calc_profit(const int &starting_year)
    -> std::tuple<double, double, double> {
  if (year < 0) {
    year = starting_year;
    fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
               " [{}] starting_investment={}\tinvestment_rate={}\n", __func__,
               cur_pfolio, investment);
  }

  if (year >= static_cast<int>(historical_returns.size())) {
    fmt::print("[{}] future year than now\n", __func__);
    return {0.0, 0.0, 0.0};
  }
  auto last_year_pfolio = cur_pfolio;
  // cur_pfolio += (cur_pfolio)*yearly_return;
  cur_pfolio += (cur_pfolio)*historical_returns[year];
  year++;
  auto tmp = cur_pfolio;
  cur_pfolio += investment * 12;
  out_of_pocket_money += investment * 12;

  return {cur_pfolio, (tmp - last_year_pfolio), out_of_pocket_money};
};

auto calc_profit_avg([[maybe_unused]] const int &starting_year)
    -> std::tuple<double, double, double> {
  auto last_year_pfolio = cur_pfolio;
  cur_pfolio += (cur_pfolio)*yearly_return;
  year++;

  auto tmp = cur_pfolio;
  cur_pfolio += investment * 12;
  out_of_pocket_money += investment * 12;

  return {cur_pfolio, (tmp - last_year_pfolio), out_of_pocket_money};
};

template <typename FUNC>
auto func(const int &starting_point, const int &investment_period, FUNC f) {
  year = -1;
  cur_pfolio = 5000;
  out_of_pocket_money = 5000;

  int yrs = 0;
  for (;;) {
    yrs++;
    std::tuple<double, double, double> profit;
    if (f == calc_profit_avg)
      profit = calc_profit_avg(10);
    else {
      profit = calc_profit(starting_point);
    }
    auto profit_from_investment =
        std::get<0>(profit) -
        std::get<2>(profit); // profit from all investments = current portofolio
                             // value - all money I invested

#if 0
    fmt::print("out_of_pocket_money={}\tcur_pfolio={}\tprofit={}\t\t({} "
               "yr(s)) per_month={}\n",
               std::get<2>(profit), std::get<0>(profit), std::get<1>(profit),
               yrs, ((std::get<1>(profit) * 1.0) / 12.0));
#endif
    if (yrs == investment_period) {
      if (profit_from_investment > 0)
        fmt::print(fmt::emphasis::bold | fg(fmt::color::green),
                   "Yearly profit={:.2f} {:.2f}%\n\n", profit_from_investment,
                   (std::get<0>(profit) / (std::get<2>(profit))));
      else
        fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
                   "Yearly profit={:.2f} {:.2f}%\n\n", profit_from_investment,
                   100.0 * (profit_from_investment / (std::get<2>(profit))));
    }
    if (yrs >= static_cast<int>(investment_period))
      // if (yrs >= historical_returns.size())
      break;
  }
};

auto take_rest() {

  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
  }
}

auto main(int args, char *argv[]) -> int {
  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
             "[{}] starting year=X for 18 years, ", __func__);
  func<std::tuple<double, double, double>(const int &)>(0, 18, calc_profit_avg);
  fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
             "[{}] starting year=X for 12 years, ", __func__);
  func<std::tuple<double, double, double>(const int &)>(0, 12, calc_profit_avg);

  for (auto i = 0; i < (static_cast<int>(historical_returns.size()) - 10);
       i++) {
    fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
               "[{}] starting year={} for 18 years, ", __func__, i);
    func<std::tuple<double, double, double>(const int &)>(i, 18, calc_profit);
    fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
               "[{}] starting year={} for 12 years, ", __func__, i);
    func<std::tuple<double, double, double>(const int &)>(i, 12, calc_profit);

    //        fmt::print(fmt::emphasis::bold | fg(fmt::color::blue),
    //                "[{}] starting year={} for 5 years", __func__, i);
    //        func(i, 5);
    fmt::print("\n\n");
    take_rest();
  }
  return 0;
}
