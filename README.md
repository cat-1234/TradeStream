## TradeStream
#### Streaming trades to aggregate statistics per symbol

(Work In Progress - some components are being redesigned and not connected)

- ### Design:
  - Principle:
    - Distributed set-up to allow scaling
      - multiple streams being read in parallel
      - multiple aggregations being run in parallel (and for each agg calc multiple aggregators, eventually...)
      - decoupled DataStore to allow scaling
      - multiple writers writing to different locations
  
  - Hierarchy:
    - FileHandlers (Reader / Writer)
    - TradeStreamer (Owns Reader, Serializer, ZMQHandle; uses reader to process trades, serializer to serialize, zmqpub to send them out to aggs)
      - (owns unique_ptr to ZMQPub to multicast trades)
    - AggStreamer (Owns writer, serializer, ZMQHandle; uses zmq to receive trades from DataStore after aggregation, uses serializer to deserialize, uses writer to write to output file)
    - Agg
      - (owns unique_ptrs to ZMQSub - recv trades from TradeStreamer; ZMQPush, ZMQPair to push req to connect, pull initial value, and push new value to DB)
    - ZMQHandler (Handle around ZMQ node setting up context and sockets and handling cleanup)
      - TCPZMQHandler (Uses tcp protocol (necessary for completely separate threads - eventual extension to multiprocess))
        - TCPZMQPull
        - TCPZMQPush
        - TCPZMQPub
        - TCPZMQSub
        - TCPZMQPair
        - TCPZMQPull/PushMulti (different connections modes for queuing)
        - TCPZMQPair for one-to-one
        - (other set-ups would be much more ideal; still more to do)
    - DataStore
      - (owns ZMQPull to pull connection req from all connected aggs)
      - (owns vector<ZMQPair> to respond to aggs req for initial value on symbol, and send back update (ds_id included in agg req gives loc of that agg's pair))
      - Current handshake: one-way (not ideal); DS waits for sign-up message from number of assigned aggs before listening for requests
      - DS owns unordered_map<std::string, std::unordered_map<std::string, std::vector<long double>>>
        - Current set-up is aggset_name -> symbol -> values (maybe not ideal, symbol -> aggset_name -> values would help with unique symbol lookup)
        - sorted vec of unique symbols: each new one added the first time it shows up in req
        - set of unique symbols for lookup
        
        ((
        
        - Matching process when agg requests conn: check if aggset_name exists, if not add to map:
          - e.q. we set up:
            - one set of AverageAggs to aggregate average price
            - another set of AverageAggs to agg average quantity
            - We choose "pAgg" as name of one set, "qAgg" for other
            - Set "pAgg" gets assigned one map, "qAgg" another
            - Loose handshake with zmq for ipc.
            
        )) - not implemented yet, will need something similar to have multiple aggs on same aggset_name
        
    - Serializer
      - serialize() and deserialize() and deserialize_one() (for picking out just one value from full trade data) and deserialize_int() for trunc
      - ChunkLenSerializer
        - using char[] as the message frame, and first n locs to store length info for n blocks, fixed nchunks member
      - VarChunksSerializer
        - uses first loc to store number of chunks: more flexible

  - ### Process Flow:
    - TradeStreamer:
      - reads input csv line by line with reader
      - serializes for zmq send with serializer
      - multicasts trades with zmqpub
    - Aggregators (Average on price, LargestRange on t, BiggestAmount on q, etc.):
      - receive trade data with zmq
      - deserialize with serializer type: set to idx of trade to agg on: e.g. constructing a SumAgg with idx = 2 would sum quantity values
      - send req to DataStore on symbol including ds_id for its own pair connection in ds
      - receive initial value and necessary prev (e.g. N trades for average calc)
      - aggregate
      - send new value to DS
    - DataStore:
      - initializes connections with aggs
      - receives requests from aggs on symbol
      - retrieves initial value sends initial value and receives new value and updates
      - sets up separate thread streaming data to AggStream for writing
    - AggStream:
      - receives data from DS
      - deserializes with serializer type
      - writes with writer


                
