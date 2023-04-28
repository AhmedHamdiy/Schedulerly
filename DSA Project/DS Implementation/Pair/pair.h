#pragma once
template <class T1=int, class T2=int>
class Pair {
public:
	T1 first;
	T2 second;
	Pair() {}
	Pair(T1 x, T2 y) :first(x), second(y) {}
	void swap(Pair<T1, T2>& p1);
	friend Pair<T1, T2> make_pair(T1 x, T2 y);
	friend bool operator == (Pair<T1, T2>& p1, Pair<T1, T2>& p2);
};

//Make two variables a pair
template <class T1, class T2>
inline Pair<T1, T2> make_pair(T1 x, T2 y) {
	return Pair< T1, T2>(x, y);
}


//Check if Two pairs are identecal
template <class T1, class T2>
inline bool operator ==(Pair<T1, T2>& p1, Pair<T1, T2>& p2) {
	return ((p1.first == p2.first) && (p1.second == p2.second));
}

//Swap the content of two pairs
template <class T1, class T2>
inline void Pair<T1,T2>::swap(Pair<T1, T2>& p1){
	T1 temp1 = p1.first;
	T2 temp2 = p1.second;
	p1.frist = first;
	p1.second = second;
	first = temp1;
	second = temp2;
}
