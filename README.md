# final-project-Eric-Zheng29<br>
## Trojan Map<br>
final-project-Eric-Zheng29 created by GitHub Classroom<br>
Team member: Tao Zheng, Team Name: Accepted<br>

---

### Overview<br>
This project focuses on using data structures and graph search algorithms to build a mapping application. The input is a map of USC campus. When you open this program, it will show you a menu, which includes 6 choices:<br>
```shell
**************************************************************
* Select the function you want to execute.
* 1. Autocomplete
* 2. Find the position
* 3. CalculateShortestPath
* 4. Traveling salesman problem(Brute force)
* 5. Traveling salesman problem(2_OPT)
* 6. Exit
**************************************************************
```
When you type a number in the menu, it will go to the function. You need follow the function instruction and it will give you teh reuslt you want. Here is a little demo of how this program works.
<p align="center"><img src="img/demo.gif" alt="TSP videos" width="500"/></p>

---

### Data Structure
Each point on the map is represented by the class **Node** shown below and defined in [trojanmap.h](src/lib/trojanmap.h).

```cpp
class Node {
  public:
    std::string id; // A unique id assign to each point
    double lat;     // Latitude
    double lon;     // Longitude
    std::string name; // Name of the location. E.g. "Bank of America".
    std::vector<std::string> neighbors; // List of the ids of all neighbor points.
};
```
---

### Function1:Autocomplete
```c++
std::vector<std::string> Autocomplete(std::string name);
```
When you want to run this function, you can type the partial name of the location and it will return a list of possible locations with partial name as prefix. Uppercase and lower case are treated as the same character.<br>
<p align="center"><img src="img/f1.png" alt="TSP videos" width="500"/></p>

#### Pseudocode
```
Input a string;
transform the input string to upper case or lower case;
iterate all nodes:
  transform node.name as input string;
  if node.name contains input:
    return node.name
```
#### Examples of function1
<p align="center"><img src="img/f1-1.png" alt="TSP videos" width="500"/></p>
<p align="center"><img src="img/f1-2.png" alt="TSP videos" width="500"/></p>
<p align="center"><img src="img/f1-3.png" alt="TSP videos" width="500"/></p>
<p align="center"><img src="img/f1-4.png" alt="TSP videos" width="500"/></p>

#### Discussion
When I implementation this functiuon first time, I didn't see the claim, "a list of possible locations with partial name as **prefix**", I miss the key word Prefix, so I return the all nodes' name which contain the input string. Finally, I change the find() third parameter from end() to 0, and then it can just return the right results.

### Function2: 
