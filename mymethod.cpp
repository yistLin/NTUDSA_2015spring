#include "mymethod.h"

ContainerA::ContainerA(){
	adid = queryid = position = depth = click = impression = 0;
}

ContainerA::~ContainerA(){}

void ContainerA::set(const int& a, const int& q, const int& p, const int& d, const int& c, const int& i){
	adid = a;
	queryid = q;
	position = p;
	depth = d;
	click = c;
	impression = i;
}

ContainerB::ContainerB(){
	url = 0;
	advertiserid = keywordid = titleid = descriptionid = userid = 0;
}

ContainerB::~ContainerB(){}

void ContainerB::set(const unsigned long long int& u,const int& a,const int& k,const int& t,const int& d,const int& uid){
	url = u;
	advertiserid = a;
	keywordid = k;
	titleid = t;
	descriptionid = d;
	userid = uid;
}

bool SmallContainer::operator<(const SmallContainer& a) const{
	if(adid < a.adid)
		return true;
	else if(adid == a.adid){
		if(queryid < a.queryid)
			return true;
		else
			return false;
	}
	else
		return false;
}

void get(std::vector<ContainerA> *v,const int& u,const int& a,const int& q,const int& p,const int& d){
	int sumOfClick = 0, sumOfImpression = 0;
	for(vector<ContainerA>::iterator it = v[u].begin(); it!=v[u].end(); ++it){
		if((*it).adid == a && (*it).queryid == q && (*it).position == p && (*it).depth == d){
			sumOfClick += (*it).click;
			sumOfImpression += (*it).impression;
		}
	}
	printf("********************\n");
	printf("%d %d\n",sumOfClick,sumOfImpression);
	printf("********************\n");
	return;
}

void clicked(vector<ContainerA> *v,const int& u){
	set<SmallContainer> s;
	SmallContainer sc;
	for(vector<ContainerA>::iterator it = v[u].begin(); it!=v[u].end(); ++it){
		if((*it).click > 0){
			sc.adid = (*it).adid;
			sc.queryid = (*it).queryid;
			s.insert(sc);
		}
	}
	printf("********************\n");
	for(set<SmallContainer>::iterator it = s.begin(); it!=s.end(); ++it){
		printf("%d %d\n",(*it).adid,(*it).queryid);
	}
	printf("********************\n");
	return;
}

void impressed(vector<ContainerA>* v,vector<ContainerB>* v2,const int& u1,const int& u2){
	set<int> sorted_adid_u1;
	set<int> sorted_adid_u2;
	vector<ContainerB> ad_properties;
	for(vector<ContainerA>::iterator itA = v[u1].begin(); itA != v[u1].end(); ++itA){
		if((*itA).impression > 0){
			sorted_adid_u1.insert((*itA).adid);
		}
	}
	for(vector<ContainerA>::iterator itB = v[u2].begin(); itB != v[u2].end(); ++itB){
		if((*itB).impression > 0){
			sorted_adid_u2.insert((*itB).adid);
		}
	}

	set<int>::iterator itA = sorted_adid_u1.begin();
	set<int>::iterator itB = sorted_adid_u2.begin();
	bool used = false;
	printf("********************\n");
	while(itA != sorted_adid_u1.end() && itB != sorted_adid_u2.end()){
		if(*itA < *itB)
			itA++;
		else if(*itA > *itB)
			itB++;
		else{
			printf("%d\n", *itA);
			ad_properties.clear();
			for(vector<ContainerB>::iterator iter=v2[*itA].begin();iter!=v2[*itA].end();++iter){
				if((*iter).userid == u1 || (*iter).userid == u2)
					ad_properties.push_back(*iter);
			}
			for(vector<ContainerB>::iterator iter=ad_properties.begin();iter!=ad_properties.end();++iter){
				used = false;
				for(vector<ContainerB>::iterator it=ad_properties.begin();it!=iter;++it){
					if((*it).url==(*iter).url && (*it).advertiserid==(*iter).advertiserid && (*it).keywordid==(*iter).keywordid && (*it).titleid==(*iter).titleid && (*it).descriptionid==(*iter).descriptionid){
						used = true;
						break;
					}
				}
				if(used == false)
					printf("\t%llu %d %d %d %d\n",(*iter).url,(*iter).advertiserid,(*iter).keywordid,(*iter).titleid,(*iter).descriptionid);
			}
			itA++;
			itB++;
		}
	}
	printf("********************\n");
	return;
}

void profit(vector<ContainerA>* v,vector<ContainerB>* v2,const int& a,const double& theta){
	set<int> sorted_userid;
	for(vector<ContainerB>::iterator it=v2[a].begin();it!=v2[a].end();++it){
		sorted_userid.insert((*it).userid);
	}

	int sumOfClick = 0, sumOfImpression = 0;
	double click_through_rate = 0;

	printf("********************\n");
	for(set<int>::iterator its=sorted_userid.begin();its!=sorted_userid.end();++its){
		sumOfClick = sumOfImpression = 0;
		for(vector<ContainerA>::iterator it=v[*its].begin(); it!=v[*its].end(); ++it){
			if((*it).adid == a){
				sumOfClick += (*it).click;
				sumOfImpression += (*it).impression;
			}
		}
		if(sumOfImpression == 0)
			click_through_rate = 0;
		else
			click_through_rate = (double)sumOfClick / (double)sumOfImpression;

		if(click_through_rate >= theta){
			printf("%d\n",(*its));
		}
	}
	printf("********************\n");
	return;
}