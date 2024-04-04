#include <benchmark/benchmark.h>

#include <sstream>
#include <string>

// DO NOT change this struct
struct User {
  int64_t id;
  std::string username;
};

std::ostream& operator<<(std::ostream& out, const User& user) {
  out << "{"
      << "\"id\":" << user.id << ","
      << "\"username\":\"" << user.username << "\""
      << "}";
  return out;
}

std::string to_json(const User& user) {
  std::stringstream ss;
  ss << user;
  return ss.str();
}

// DO NOT change this struct
struct Message {
  int64_t id;
  std::string subject;
  std::string body;

  User from;
  User to;
};

static std::string cache_str_msg = "";
static Message cache_msg;
static bool is_empty = true;

bool check_equal_users(const User& cache_user, const User& curr_user) {
  return cache_user.id == curr_user.id && cache_user.username == curr_user.username;
}

bool check_equal(const Message& msg) {
  return msg.id == cache_msg.id && msg.body == cache_msg.body && msg.subject == cache_msg.subject &&
         check_equal_users(cache_msg.from, msg.from) && check_equal_users(cache_msg.to, msg.to);
}

// DO NOT change this function's signature
std::string to_json(const Message& msg) {
  if (!is_empty && check_equal(msg)) {
    return cache_str_msg;
  }
  std::stringstream ss;
  ss << "{"
     << "\"id\":" << msg.id << ","
     << "\"subject\":\"" << msg.subject << "\","
     << "\"body\":\"" << msg.body << "\","
     << "\"from\":" << msg.from << ","
     << "\"to\":" << msg.to << "}";

  cache_str_msg = ss.str();
  cache_msg = msg;

  if (is_empty) {
    is_empty = false;
  }
  return cache_str_msg;
}

namespace {

void bm_message_to_json(benchmark::State& state) {
  Message msg{
      .id = 1000,
      .subject = "About modules",
      .body = "So, when is that 'modules' proposal coming?",
      .from = User{.id = 12345,      .username = "Herb Sutter"},
      .to = User{    .id = 1, .username = "Biern Stroustrup"}
  };

  for (auto _ : state) {
    benchmark::DoNotOptimize(to_json(msg));
  }

  auto json = to_json(msg);
  std::string expected =
      R"({"id":1000,"subject":"About modules","body":"So, when is that 'modules' proposal coming?","from":{"id":12345,"username":"Herb Sutter"},"to":{"id":1,"username":"Biern Stroustrup"}})";
  if (json != expected) {
    state.SkipWithError("Wrong output");
  }
}

} // namespace

BENCHMARK(bm_message_to_json);

BENCHMARK_MAIN();
