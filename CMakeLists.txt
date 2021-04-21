project(TEMPLATE)

cmake_minimum_required(VERSION 3.12...3.19)

set(TEMPLATE_VERSION_MAJOR 0)
set(TEMPLATE_VERSION_MINOR 0)
set(TEMPLATE_VERSION_MICRO 1)

string(JOIN "." TEMPLATE_VERSION
	"${TEMPLATE_VERSION_MAJOR}"
	"${TEMPLATE_VERSION_MINOR}"
	"${TEMPLATE_VERSION_MICRO}"
)

# add_executable(ip-stats "test/ip-stats.c")
# add_executable(tcp-stats "test/tcp-stats.c")
# add_executable(browse-performance-counters "test/browse-performance-counters.c")
# add_executable(net-iface-bytes "test/net-iface-bytes.c")
