#include "src/lib/trojanmap.h"

#include <map>
#include <vector>

#include "gtest/gtest.h"

TEST(TrojanMapTest, Autocomplete) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  // Test the simple case
  auto names = m.Autocomplete("ta");
  std::vector<std::string> gt1 = {"Target", "Tap Two Blue"}; // groundtruth for "Ch"
  EXPECT_EQ(names, gt1);
  // Test the lower case
  names = m.Autocomplete("TA");
  std::vector<std::string> gt2 = {"Target", "Tap Two Blue"}; // groundtruth for "ch"
  EXPECT_EQ(names, gt2);
  // Test the lower and upper case 
  names = m.Autocomplete("tA"); 
  std::vector<std::string> gt3 = {"Target", "Tap Two Blue"}; // groundtruth for "cH"
  EXPECT_EQ(names, gt3);
}

// Test FindPosition function
TEST(TrojanMapTest, FindPosition) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  // Test ChickfilA
  auto position = m.GetPosition("Popeyes Louisiana Kitchen");
  std::string id1 = m.GetID("Popeyes Louisiana Kitchen");
  std::pair<double, double> gt1(m.GetLat(id1), m.GetLon(id1)); // groundtruth for "ChickfilA"
  EXPECT_EQ(position, gt1);
  // Test Ralphs
  position = m.GetPosition("Starbucks Coffee");
  std::string id2 = m.GetID("Starbucks Coffee");
  std::pair<double, double> gt2(m.GetLat(id2), m.GetLon(id2)); // groundtruth for "Ralphs"
  EXPECT_EQ(position, gt2);
  // Test Target
  position = m.GetPosition("Chipotle Mexican Grill");
  std::string id3 = m.GetID("Chipotle Mexican Grill");
  
  std::pair<double, double> gt3(m.GetLat(id3), m.GetLon(id3)); // groundtruth for "Target"
  EXPECT_EQ(position, gt3);
}

// Test CalculateShortestPath function 1
TEST(TrojanMapTest, CalculateShortestPath) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  // Test from Ralphs to ChickfilA
  auto path = m.CalculateShortestPath("Target", "Ralphs");
  std::vector<std::string> gt{
      "5237417650","6813379479","4015372485","7071032399","4015372486","6813405232","122719216","6813405231",
      "2613117893","122719255","441895335","441895337","213332111","6807243576","6807243574","122719259","2613117895",
      "123178876","6816193783","6389467806","6816193784","6813405269","6816831441","6808093919","6808093913","6808093910",
      "6787470571","5559640911","2578244375"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to ChickfilA
  path = m.CalculateShortestPath("Ralphs", "Target");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

// Test CalculateShortestPath function 2
TEST(TrojanMapTest, CalculateShortestPath2) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  auto path = m.CalculateShortestPath( "Popeyes Louisiana Kitchen", "Ralphs");
  // Test from Target to Popeyes Louisiana Kitchen
  std::vector<std::string> gt{
      "5695236164","4835551070","6813565322","123241961","6813565326","4012842277","4835551064","5556117115",
      "5556117120","20400292","6813513564","6813513563","122659187","7863689395","4835551084","2613117900",
      "7863656075","7863689388","6807580191","6787803674","6787803666","6787803661","6787803658","6787803653",
      "6787803649","6816822864","122659191","2613117891","6813379507","6985903602","6813416161","122814440",
      "7200139036","7882624618","6813416166","6807536642","6807320427","6807536647","6813416171","6813416123",
      "122814447","6813416159","6813405266","123178871","2613117906","2613156405","6807243572","6813405267",
      "6987230634","6987230635","123178876","6816193783","6389467806","6816193784","6813405269","6816831441",
      "6808093919","6808093913","6808093910","6787470571","5559640911","2578244375"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Popeyes Louisiana Kitchen to Target
  path = m.CalculateShortestPath("Ralphs","Popeyes Louisiana Kitchen");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}


// Test TSP function(Brute force)
TEST(TrojanMapTest, TSP) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"21098546", "33078883", "63068593", "122609808"}; // Input location ids 
  auto result = m.TravellingTrojan(input);
  std::cout << "My path length: "  << result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"21098546","63068593","122609808","33078883","21098546"}; // Expected order
  std::cout << "GT path length: "  << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) 
    flag = true;
  
  EXPECT_EQ(flag, true);
}

// Test TSP function 2(Brute force)
TEST(TrojanMapTest, TSP2) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"122659040", "122719210", "213431695", "269633660", "348122791", "544672064"}; // Input location ids 
  auto result = m.TravellingTrojan(input);
  std::cout << "My path length: " << result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"122659040","122719210","269633660","348122791","213431695","544672064","122659040"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the groundtruth path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) // counterclockwise
    flag = true;
  
  EXPECT_EQ(flag, true);
}

// Test TSP function 3(Brute force)
TEST(TrojanMapTest, TSP3) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"6818427947", "354063330", "1614922667", "1790748069", "1849116067", "2613073707", "4011837236", "4015372474", "6433913234", "6817197853"}; // Input location ids 
  auto result = m.TravellingTrojan(input);
  std::cout << "My path length: " <<result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"6818427947","6433913234","1849116067","4011837236","1614922667","6817197853","1790748069","354063330","4015372474","2613073707","6818427947"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the groundtruth path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) // counterclockwise
    flag = true;
  
  EXPECT_EQ(flag, true);
}

// Test TSP function(2_OPT) test wether each iteration improve the path distance
TEST(TrojanMapTest, TSP_2opt) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"21098546", "33078883", "63068593", "122609808"}; // Input location ids 
  auto result = m.TravellingTrojan_2opt(input);
  bool flag = true;
  double cur_dist = 0;
  double next_dist = 0;
  for(int i = 0; i < result.second.size() - 1; i++){
    cur_dist = m.CalculatePathLength(result.second[i]);
    next_dist = m.CalculatePathLength(result.second[i+1]);
    if(cur_dist < next_dist){
      flag = false;
    }
  }
  
  EXPECT_EQ(flag, true);
}

// Test TSP function 2(2_OPT)
TEST(TrojanMapTest, TSP2_2opt) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"122659040", "122719210", "213431695", "269633660", "348122791", "544672064"}; // Input location ids 
  auto result = m.TravellingTrojan_2opt(input);
  bool flag = true;
  double cur_dist = 0;
  double next_dist = 0;
  for(int i = 0; i < result.second.size() - 1; i++){
    cur_dist = m.CalculatePathLength(result.second[i]);
    next_dist = m.CalculatePathLength(result.second[i+1]);
    if(cur_dist < next_dist){
      flag = false;
    }
  }
  
  EXPECT_EQ(flag, true);
}

// Test TSP function 3(2_OPT)
TEST(TrojanMapTest, TSP3_2opt) {
  TrojanMap m;
  m.CreateGraphFromCSVFile();
  std::vector<std::string> input{"6818427947", "354063330", "1614922667", "1790748069", "1849116067", "2613073707", "4011837236", "4015372474", "6433913234", "6817197853"}; // Input location ids 
  auto result = m.TravellingTrojan_2opt(input);
  bool flag = true;
  double cur_dist = 0;
  double next_dist = 0;
  for(int i = 0; i < result.second.size() - 1; i++){
    cur_dist = m.CalculatePathLength(result.second[i]);
    next_dist = m.CalculatePathLength(result.second[i+1]);
    if(cur_dist < next_dist){
      flag = false;
    }
  }
  
  EXPECT_EQ(flag, true);
}