
#include "Artist.h"


Artist::Artist(int id, int num_of_songs) : full_songs_list((new StreamListNode*[num_of_songs])){
    artist_id = id;
    total_num_of_songs = num_of_songs;
    //create the songs tree - create an array, insert it, delete it
    int* songs_arr = new int[num_of_songs];
    for(int i = 0; i < num_of_songs; i++){
        songs_arr[i] = i;
    }
    AvlTree<int, int> tmp(songs_arr,songs_arr,num_of_songs);
    delete[] songs_arr;
    // create AvlTree for streams_tree - with first node "0" for all the songs
    num_of_streams_tree.insert(tmp,0);
}

Artist::~Artist() {
    delete[] full_songs_list;
}

Artist::Artist(const Artist& a) : artist_id(a.artist_id), total_num_of_songs(a.total_num_of_songs),
                                    num_of_streams_tree(a.num_of_streams_tree) {
    full_songs_list = new StreamListNode*[a.total_num_of_songs];
    for(int i = 0; i < a.total_num_of_songs; i++){
        full_songs_list[i] = a.full_songs_list[i];
    }
}

StreamListNode* Artist::GetSongNumOfStreamsNode(int song_id) {
    return this->full_songs_list[song_id];
}

void Artist::SetStreamsNumForSong(int song_id, StreamListNode* node){
    full_songs_list[song_id] = node;
}
