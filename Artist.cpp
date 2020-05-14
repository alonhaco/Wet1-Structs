//
// Created by User on 5/8/2020.
//

#include "Artist.h"


Artist::Artist(int id, int num_of_songs) : num_of_streams_tree(*(new AvlTree<AvlTree<int,int>,int>())),
                                           full_songs_list((new StreamListNode*[num_of_songs])){

    artist_id = id;
    total_num_of_songs = num_of_songs;

    // link all songs the the Node 0 in StreamList - to implement in the AddArtist Func?

    //create the songs tree - create an array, insert it, delete it
    int songs_arr[num_of_songs];
    for(int i = 0; i < num_of_songs; i++){
        songs_arr[i] = i;
    }
    AvlTree<int,int> songs_tree = AvlTree<int,int>(songs_arr, songs_arr, num_of_songs);
    //delete[] songs_arr;

    // create AvlTree for streams_tree - with first node "0" for all the songs
    num_of_streams_tree.insert(songs_tree,0);

}

Artist::~Artist() {

    // delete full_song_list
    delete[] full_songs_list;
    // delete nodes in AvlTree for streams_tree - use the destructor?
    delete(&num_of_streams_tree);

}


StreamListNode* Artist::GetSongNumOfStreamsNode(int song_id) {
    return this->full_songs_list[song_id];
}

void Artist::SetStreamsNumForSong(int song_id, StreamListNode* wanted_node){
    full_songs_list[song_id] = wanted_node;
}



















/*
class Song{
private:
    int song_id;
    *int* num_of_times_streamed; // should point to the list?
public:
    Song();
    ~Song();
    Song(const Song& s) = delete;
    Song& operator=(const Song& s) = delete;

    editors:
            change num_of_times_streamed
};

Song::Song() {
    song_id = ;
    // pointer should point to 0 in the linked list
}

Song::~Song() {

}
*/


// what about the list of streams? should I write a new linked list?

