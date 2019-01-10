#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
struct Individual {
    string chromosome;
    double fitness;
    Individual(string s = "", double f = 0) : chromosome(s), fitness(f) {}
};
string itos(int a) {
    string s;
    while (a) {
        s += a % 10 + '0';
        a /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}
default_random_engine gen(time(NULL));
uniform_int_distribution<int> dis;
uniform_real_distribution<double> dis_r(0.0, 1.0);
class Genetic_algorithm {
   private:
    vector<Individual> Population;
    int Population_size;              //种群数量
    int Chromosome_size;              //染色体长度
    int Lower;                        //求解的左区间
    int Upper;                        //求解的右区间
    int Accuracy;                     //要求结果的精度
    int range;                        //生成编码的范围
    double Probability_of_crossover;  //交叉率
    double Probability_of_mutation;   //突变率
    double Maxfitness;                //当前种群的最优适应度

   public:
    Genetic_algorithm(int lower, int upper, int acc, double pc, double pm) {
        Upper = upper;
        Lower = lower;
        Accuracy = acc;
        Probability_of_crossover = pc;
        Probability_of_mutation = pm;
        Population_size = (upper - lower) * 100;      //种群数量为300
        range = (Upper - Lower) * pow(10, Accuracy);  //计算编码的范围
        Chromosome_size = log2(range) + 1;            //计算染色体长度
        Maxfitness = INT_MIN;
    }
    void Firstgen() {
        for (int i = 0; i < Population_size; i++) {
            int tmp = dis(gen) % (range + 1);
            Individual s;
            for (int j = 0; j < Chromosome_size; j++) {
                s.chromosome += '0' + (tmp & 1);
                tmp >>= 1;
            }
            s.fitness = Fitness_function(s.chromosome);
            Population.push_back(s);
        }
    }
    double Decode(string s) {  //解码
        int p = 1, res = 0;
        for (int i = 0; i < Chromosome_size; i++) {
            if (s[i] - '0') {
                res += p;
            }
            p <<= 1;
        }
        return (double)res / pow(10, Accuracy) + Lower;
    }
    double Fitness_function(string s) {
        double x = Decode(s);
        return x * sin(10 * acos(-1) * x) + 1;
    }
    int Selection() {  //四元竞争选择
        int p = dis(gen) % Population_size;
        for (int i = 0; i < 3; i++) {
            int tmp = dis(gen) % Population_size;
            if (Fitness_function(Population[p].chromosome) <
                Fitness_function(Population[tmp].chromosome)) {
                p = tmp;
            }
        }
        return p;
    }
    void Crossover() {
        vector<Individual> new_gen;
        int n = 0;
        while (n < Population_size) {
            if (dis_r(gen) < Probability_of_crossover) {  //交叉概率
                int a = Selection(), b = Selection();  //选择并进行交叉
                int p = dis(gen) % Chromosome_size;
                string s1 = Population[a].chromosome.substr(0, p) +
                            Population[b].chromosome.substr(p, Chromosome_size);
                string s2 = Population[b].chromosome.substr(0, p) +
                            Population[a].chromosome.substr(p, Chromosome_size);
                Mutate(s1);  //突变
                Mutate(s2);
                if (Decode(s1) <= 2.0 && Decode(s1) >= -1.0 &&
                    Decode(s2) <= 2.0 &&
                    Decode(s2) >= -1.0) {  //检查个体是否越界
                    new_gen.push_back(Individual(s1, Fitness_function(s1)));
                    new_gen.push_back(Individual(s2, Fitness_function(s2)));
                    n += 2;
                }
            } else {
                new_gen.push_back(
                    Population[Selection()]);  //不交叉直接进入下一代
                new_gen.push_back(Population[Selection()]);
                n += 2;
            }
        }
        Population.assign(new_gen.begin(), new_gen.end());
    }
    void Mutate(string &s) {
        for (int i = 0; i < Chromosome_size; i++) {
            if (dis_r(gen) < Probability_of_mutation) {
                if (s[i] == '0') {
                    s[i] = '1';
                } else
                    s[i] = '0';
            }
        }
    }
    void solve() {
        Firstgen();  //生成第一代种群
        int generation = 0, cnt = 0;
        string address2 = "fitness/", address3 = "result/",
               name = "generation ";
        for (int i = 0; i < 50; i++) {
            if (cnt >= 10) break;
            generation++;
            ofstream file1(address2 + name + itos(generation)),
                resfile(address3 + name +
                        itos(generation));  //创建文件夹，存储数据
            Crossover();                    //交叉
            Maxfitness = INT_MIN;
            for (int j = 0; j < Population_size; j++) {
                Maxfitness = max(Maxfitness, Population[j].fitness);
                file1 << fixed << setprecision(6)
                      << Decode(Population[j].chromosome) << ','
                      << Population[j].fitness
                      << endl;  //求最优适应度，并将数据输出到文件
            }
            file1.close();
            resfile << fixed << setprecision(7) << Maxfitness << endl;
            resfile.close();
        }
        cout << "result:" << fixed << setprecision(7) << Maxfitness << endl;
    }
};
int main() {
    clock_t t;  //记录程序运行时间
    t = clock();
    Genetic_algorithm demo(-1, 2, 7, 0.7, 0.01);
    demo.solve();
    t = clock() - t;
    cout << "run time: " << fixed << setprecision(1)
         << (float)t / CLOCKS_PER_SEC << " seconds" << endl;
}