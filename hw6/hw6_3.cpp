#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

vector< vector<long long int> > game_set;
vector<int> track_owner;
vector<int> real_owner;

int findOwner(const int owner) {
  if (owner == track_owner[owner])
    return owner;
  else {
    track_owner[owner] = findOwner(track_owner[owner]);
    return track_owner[owner];
  }
}

void union_by_size(int a, int b) {
  int br = real_owner[a]; // br = borrower
  int lr = real_owner[b]; // lr = lender

  if (game_set[br].size() >= game_set[lr].size()) {
    track_owner[b] = a;
  } else {
    track_owner[a] = b;
    real_owner[b] = br;
  }
  game_set[br].insert(game_set[br].end(), game_set[lr].begin(), game_set[lr].end());
  game_set[lr].clear();
}

int buyMoreGames(int owner, long long int s) {
  owner = real_owner[owner];
  sort(game_set[owner].begin(), game_set[owner].end());
  long long int sum = 0;
  int iter = 0;
  while (iter < game_set[owner].size()) {
    sum += game_set[owner].at(iter);
    if (sum > s) {
      break;
    }
    iter++;
  }
  return iter;
}

int main(int argc, char const *argv[])
{
  int n, m;
  int cmd, game_i, game_j, owner_i, owner_j;
  long long int p, s;
  
  scanf("%d%d", &n, &m);

  game_set.resize(n + 1);
  track_owner.resize(n + 1);
  real_owner.resize(n + 1);

  for (int i = 1; i <= n; ++i) {
    scanf("%lld", &p);
    game_set.at(i).push_back(p);
    track_owner.at(i) = i;
    real_owner.at(i) = i;
  }

  for (int _process = 0; _process < m; ++_process) {
    scanf("%d", &cmd);

    if (cmd == 1) {
      // find and merge
      scanf("%d%d", &game_i, &game_j);
      owner_i = findOwner(game_i);
      owner_j = findOwner(game_j);

      if (owner_i != owner_j) {
        union_by_size(owner_i, owner_j);
      }

    } else if (cmd == 2) {
      // find and sum price
      scanf("%d%lld", &game_i, &s);
      owner_i = findOwner(game_i);
      printf("%d %d\n", real_owner[owner_i], buyMoreGames(owner_i, s));
    }
  }

  return 0;
}
