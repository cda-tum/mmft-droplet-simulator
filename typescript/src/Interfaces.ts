interface ChannelPosition {
    channelId: number;
    position: number;
};

enum BoundaryState {
    NORMAL,
    WAIT_INFLOW,
    WAIT_OUTFLOW
};

interface DropletBoundary {
    position: ChannelPosition;
    volumeTowards0: boolean;
    flowRate: number;
    state: BoundaryState;
};

interface DropletPosition {
    boundaries: DropletBoundary[];
    channelIds: number[];
    state: DropletState;
};

interface Fluid {
    id: number;
    name: string;
    viscosity: number
    density: number
    concentration: number
    mixedFluidIds: number[];
};

interface Droplet {
    id: number;
    name: string;
    volume: number;
    fluidId: number;
    mergedDropletIds: number[];
};

interface Injection {
    id: number;
    dropletId: number;
    time: number;
    position: ChannelPosition;
};

enum ChannelType {
    NORMAL,
    BYPASS,
    CLOGGABLE
};

enum DropletState {
    INJECTION,
    NETWORK,
    TRAPPED,
    SINK
};

interface Channel {
    id: number;
    name: string;
    node0Id: number;
    node1Id: number;
    width: number;
    height: number;
    length: number;
    type: ChannelType
};

interface FlowRatePump {
    id: number;
    name: string;
    node0Id: number;
    node1Id: number;
    flowRate: number;
};

interface PressurePump {
    id: number;
    name: string;
    node0Id: number;
    node1Id: number;
    pressure: number;
};

interface Chip {
    name: string;
    channels: Channel[];
    flowRatePumps: FlowRatePump[];
    pressurePumps: PressurePump[];
};

interface State {
    id: number;
    time: number;
    pressures: Map<number, number>;
    flowRates: Map<number, number>;
    dropletPositions: Map<number, DropletPosition>;
};

export interface SimulationResult {
    continuousPhaseId: number;
    maximalAdaptiveTimeStep: number;
    resistanceModel: number;
    chip: Chip;
    fluids: Fluid[];
    droplets: Droplet[];
    injections: Injection[];
    states: State[];
};