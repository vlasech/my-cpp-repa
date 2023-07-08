#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <iostream>

//#include <gtest/gtest.h>

/*
TEST(TransportCatalogueTest, TestAddAndGetStop)
{
    TransportCatalogue tc;
    Stop stop{"Test Stop", 55.75222, 37.61556};

    tc.AddStop(stop);

    Stop *addedStop = tc.GetStopByName("Test Stop");
    ASSERT_NE(addedStop, nullptr);
    EXPECT_EQ(addedStop->name, "Test Stop");
 //   EXPECT_EQ(addedStop->latitude, 55.75222);
   // EXPECT_EQ(addedStop->longitude, 37.61556);
}

TEST(TransportCatalogueTest, TestAddAndGetBus)
{
    TransportCatalogue tc;
    Bus bus{"Bus Test", {}};

    tc.AddBus(bus);

    Bus *addedBus = tc.GetBusByName("Bus Test");
    ASSERT_NE(addedBus, nullptr);
    EXPECT_EQ(addedBus->name, "Bus Test");
    EXPECT_TRUE(addedBus->stops.empty());
}

TEST(TransportCatalogueTest, TestGetNonexistentStop)
{
    TransportCatalogue tc;

    Stop *nonExistentStop = tc.GetStopByName("Nonexistent");
    EXPECT_EQ(nonExistentStop, nullptr);
}

TEST(TransportCatalogueTest, TestGetNonexistentBus)
{
    TransportCatalogue tc;

    Bus *nonExistentBus = tc.GetBusByName("Nonexistent");
    EXPECT_EQ(nonExistentBus, nullptr);
}

*/
int main()
{
    using namespace transport;
    TransportCatalogue catalogue;

    detail::ProcessRequests(catalogue, detail::ReadRequests(std::cin));

    detail::ProcessUserRequests(catalogue, detail::ReadUserRequests(std::cin), std::cout);

    return 0;
}
