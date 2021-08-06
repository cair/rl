//
// Created by per on 8/5/21.
//

#ifndef CLASSIC_CONTROL_VECTORWRAPPER_H
#define CLASSIC_CONTROL_VECTORWRAPPER_H

namespace cairl::utils{

template<class T>
    class VectorWrapper{
    public:
        using DType = std::vector<T>;
        DType _data;

        std::vector<T>& data(){
            return _data;
        }

        const std::vector<T>& data() const{
            return _data;
        }

        constexpr VectorWrapper(){}

        constexpr VectorWrapper(int size)
        : _data(size){
        }

        constexpr VectorWrapper(std::initializer_list<DType> fwlist)
        : _data(fwlist.begin()){

        }

        T& operator [](int x){
            try{
                return _data.at(x);
            }catch(const std::exception&/*e*/){
                _data.reserve(x+1);
            }
            return _data[x];
        }

        T min() const{
            return 1;
        }

        T max() const{
            return 1;
        }

    };

}




#endif //CLASSIC_CONTROL_VECTORWRAPPER_H
